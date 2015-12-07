<?php
class ModelMtbEstadisticas extends MainModel implements InterfaceModel
{
    protected $_code;
    private $_code_account;
    private $_code_agent;
    private $_code_familia;
    private $_code_subfamilia;
    private $_empresa;
    private $_mes;
    private $_ventasact;
    private $_ventasant;
    private $_code_erp;
    
    private $_id;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    private $_modify_date;
    private $_modify_user;
    
    private $_arMeses = array();
    private $_arDinamicLabels = array();
    private $_arParamsList = array();
    
    public function __construct($code_user_session="",$code="", 
        $code_account="", $code_agent="", $code_erp="", $code_familia="", $code_subfamilia="",
        $empresa="", $mes="", $ventasact="", $ventasant="",
        $create_date="", $create_user="", $modify_user="", $modify_date="", 
        $delete_date="", $delete_user="", $id="")
    {
        global  $oDB;

        $this->oDB = $oDB;
        $this->_table_name = "MTB_Estadisticas";
        $this->_table_code = "";
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT  Code, Empresa, Code_Familia, Code_SubFamilia, Mes, VentasAct, VentasAnt, 
                    Code_Erp, Code_Account, Code_Agent, 
                Id, Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date, 
                FROM MTB_Estadisticas";
        $this->_default_sql = $sSQL;

        $this->_code = $code;
        $this->_code_account = $code_account;
        $this->_code_agent = $code_agent;
        $this->_code_familia = $code_familia;
        $this->_code_subfamilia = $code_subfamilia;
        $this->_empresa = $empresa;
        $this->_mes = $mes;
        $this->_ventasact = $ventasact;
        $this->_ventasant = $ventasant;
        $this->_code_erp = $code_erp;

        $this->_id = $id;
        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_delete_date = $delete_date;
        $this->_delete_user = $delete_user;
    }

    public function update(){}
    public function delete(){}
    public function insert(){}
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arData["Code"];
        $this->_code_account = $arData["Code_Account"];
        $this->_code_agent = $arData["Code_Agent"];
        $this->_code_familia = $arData["Code_Familia"];
        $this->_code_subfamilia = $arData["Code_SubFamilia"];
        $this->_empresa = $arData["Empresa"];
        $this->_mes = $arData["Mes"];
        $this->_ventasact = $arData["VentasAct"];
        $this->_ventasant = $arData["VentasAnt"];
        $this->_code_erp = $arData["Code_Erp"];

        $this->_id = $arRow["Id"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];
    }

    private function arrange_fields_to_show($arOrderFields,$arFieldNames)
    {
        //bug($arOrderFields); die;
        $arOrdered = array();
        //$arOrder debe ser mayor a $arFieldNames
        foreach($arOrderFields as $i=>$sNameOrder)
        {
            if(in_array($sNameOrder,$arFieldNames))
            {
                $arOrdered[] = $sNameOrder;
            }
        }
        return $arOrdered;
    }
    
    public function get_mobile_list_by_account()
    {
        if(is_array($this->_arMeses) && key_exists(0,$this->_arMeses) && $this->_arMeses[0]=="") 
            unset($this->_arMeses[0]);
        $sCodeAgent = $this->_code_agent;
        $sCodeAccount = $this->_code_account;
        
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;

        //Orden de como se deben visualizar las columnas en la cabecera
        $arLabelsOrder = array("Fam.","Subfam.","Mes","V Act.","V Ant.");

        //Orden que deben seguir las columnas al mostrarse en el listado
        $arFieldOrder = 
        array("AutoId","Code_Familia, Familia_Description","Code_SubFamilia, Subfamilia_Description","Mes, Mes_Description","Mes='todos', Mes_Description='Todos'","VentasAct","VentasAnt");

        $arFieldsSub = array("AutoId","VentasAct","VentasAnt");
        //Campos que siempre se muestran en el listado
        $arFields = array("CONVERT(DECIMAL(12,2),SUM(est.[VentasAct])) as VentasAct", "CONVERT(DECIMAL(12,2),SUM(est.[VentasAnt])) as VentasAnt");
        //Campos por los que se agrupara en la subconsulta
        $arGroupBy = array();
        //Campos por los que se ordenará en la consulta exterior
        $arOrderBy = array();
        //Condiciones tipo AND que se añadirán al Vendedor y Cuenta
        $arWhereAnd = array();
        //Los campos que formarán el id único. Necesario para poder paginar
        $arAutoId = array();
        //Los nombres de las etiquetas de la cabecera del listado. Por defecto siempre se muestran estas.
        $arDinamicLabels = array("VentasAct"=>"V Act.","VentasAnt"=>"V Ant.");
        
        //LISTADO Sin filtros.  Este listado es equivalente a la aplicación de filtros: Familia:Todas y SubFam:Todas
        if(empty($this->_code_familia)&&empty($this->_code_subfamilia)&&empty($this->_arMeses))
        {
            $arFieldsSub[] = "Code_Familia, Familia_Description";
            $arFieldsSub[] = "Code_SubFamilia, Subfamilia_Description";
            $arFieldsSub[] = "Mes, Mes_Description";
            
            $arAutoId[] = "est.[Code_Familia]";
            $arAutoId[] = "est.[Code_SubFamilia]";
            $arAutoId[] = "est.[Mes]";
            
            $arFields[] = "est.[Code_Familia] as Code_Familia, pf.[Description] as Familia_Description\n";
            $arFields[] = "est.[Code_SubFamilia] as Code_SubFamilia, ps.[Description] as Subfamilia_Description\n";
            $arFields[] = "est.[Mes] as Mes, meses.[Description] as Mes_Description\n";
            
            $arGroupBy[] = "est.[Code_Familia],pf.[Description],est.[Code_SubFamilia],ps.[Description],est.[Mes],meses.[Description]";
            $arOrderBy[] = "Familia_Description, Subfamilia_Description, Mes";
            
            $arDinamicLabels["Familia"] = "Fam.";
            $arDinamicLabels["Subfamilia"] = "Subfam.";
            $arDinamicLabels["Mes"] = "Mes";
        }
        //Hay filtros
        else
        {
            if(!empty($this->_code_familia)) 
            {   
                $arFieldsSub[] = "Code_Familia, Familia_Description";
                
                $arAutoId[] = "est.[Code_Familia]";
                $arFields[] = "est.[Code_Familia] as Code_Familia, pf.[Description] as Familia_Description\n";
                if($this->_code_familia!="todas") $arWhereAnd[] = "est.[Code_Familia]='$this->_code_familia'";
                $arGroupBy[] = "est.[Code_Familia],pf.[Description]";
                $arOrderBy[] = "Familia_Description";
                $arDinamicLabels["Familia"] = "Fam.";
            }

            if(!empty($this->_code_subfamilia)) 
            {   
                $arFieldsSub[] = "Code_SubFamilia, Subfamilia_Description";
                
                $arAutoId[] = "est.[Code_SubFamilia]";
                $arFields[] = "est.[Code_SubFamilia] as Code_SubFamilia, ps.[Description] as Subfamilia_Description\n";
                if($this->_code_subfamilia!="todas")
                    $arWhereAnd[] = "est.[Code_Subfamilia] = '$this->_code_subfamilia'";
                $arGroupBy[] = "est.[Code_SubFamilia],ps.[Description]";
                $arOrderBy[] = "Subfamilia_Description";
                $arDinamicLabels["Subfamilia"] = "Subfam.";
            }
            
            if(!empty($this->_arMeses))
            {
                //Si en el listado lleva el valor todos. Tiene prioridad sobre el resto
                if(is_array($this->_arMeses)&& in_array("todos",$this->_arMeses))
                {
                    //columna mes as "Todos"
                    $arAutoId[] = "'todos'";
                    $arFieldsSub[] = "Mes='todos', Mes_Description='Todos'";
                    //$arFields[] = "Mes='todos', Mes_Description='Todos'\n";
                    //$arWhereAnd[] = "est.[Mes] IN ".$this->build_sqlin($this->_arMeses);
                    //$arGroupBy[] = "Mes, Mes_Description";
                    //$arOrderBy[] = "Mes";
                }
                //Meses concretos
                elseif(is_array($this->_arMeses) && !in_array("todos",$this->_arMeses))
                {
                    $arAutoId[] = "est.[Mes]";
                    $arFieldsSub[] = "Mes, Mes_Description";
                    $arFields[] = "est.[Mes] as Mes, meses.[Description] as Mes_Description\n";
                    $arWhereAnd[] = "est.[Mes] IN ".$this->build_sqlin($this->_arMeses);
                    $arGroupBy[] = "est.[Mes], meses.[Description]";
                    //$arOrderBy[] = "Mes";
                }
            } 
            //No hay meses seleccionados. Si ocurre esto 
            else
            {
                $arAutoId[] = "est.[Mes]";
                $arFieldsSub[] = "Mes, Mes_Description";
                $arFields[] = "est.[Mes] as Mes, meses.[Description] as Mes_Description\n";
                //$arWhereAnd[] = "est.[Mes] IN ".$this->build_sqlin($this->_arMeses);
                $arGroupBy[] = "est.[Mes], meses.[Description]";
                //$arOrderBy[] = "Mes";
            }
            $arOrderBy[] = "Mes";
            $arDinamicLabels["Mes"] = "Mes";
        }//Fin hay filtros

        //string en formato SQL
        $sCONCATID = $this->build_sql_concatid($arAutoId);
        if(!empty($arWhereAnd)) $sWHEREAnd = "AND ".join(" AND ",$arWhereAnd);
        $sGROUPBY = $this->build_sql_groupby($arGroupBy);
        $sORDERBY = $this->build_sql_orderby($arOrderBy);
        
        $this->_arDinamicLabels = $this->arrange_fields_to_show($arLabelsOrder,$arDinamicLabels);
        
        //CREANDO LA SQL
        $sFields = join(",",$arFields);
        $sFieldsSub = join(",",$arFieldsSub);
        
        $sSQL = "
        SELECT $sFieldsSub
        FROM
        (
            SELECT $sCONCATID,
            $sFields
            FROM MTB_Estadisticas AS est
            LEFT OUTER JOIN core_users AS u
            ON u.[Code]=est.[Code_Agent] 
            LEFT OUTER JOIN clients  AS c
            ON c.[Code]=est.[Code_Account] 
            LEFT OUTER JOIN MTB_Products_Family AS pf
            ON pf.[Code]=est.[Code_Familia] 
            LEFT OUTER JOIN MTB_Products_SubFamily AS ps
            ON ps.[Code]=est.[Code_SubFamilia] AND ps.[Code_Family] = est.[Code_Familia]  
            LEFT OUTER JOIN MTB_Array AS meses
            ON meses.[Code]=est.[Mes] AND meses.Type='mes' 
            WHERE 1=1
            AND est.Delete_Date IS NULL 
            AND est.[Code_Agent] = '$sCodeAgent'
            AND est.[Code_Account] = '$sCodeAccount'
            $sWHEREAnd
            $sGROUPBY
        ) AS Sub 
        $sORDERBY";
        
        //bug($sSQL); //DIE;
        $arIdPaginate = $this->oDB->query($sSQL);

        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores únicos necesarios para la paginación
        $arIdPaginate = $this->get_field_values($arIdPaginate, "AutoId");
        
        //String paginate 
        $sSQLIn = $this->build_sqlin($arIdPaginate);
        //bug($arFieldOrder); bug($arFieldsSub); die;
        $arFieldsSub = $this->arrange_fields_to_show($arFieldOrder, $arFieldsSub);
        
        $sFieldsSub = join(",",$arFieldsSub);
        $sSQL = "
        SELECT $sFieldsSub
        FROM
        (
            SELECT $sCONCATID,
            $sFields
            FROM MTB_Estadisticas AS est
            LEFT OUTER JOIN core_users AS u
            ON u.[Code]=est.[Code_Agent] 
            LEFT OUTER JOIN clients  AS c
            ON c.[Code]=est.[Code_Account] 
            LEFT OUTER JOIN MTB_Products_Family AS pf
            ON pf.[Code]=est.[Code_Familia] 
            LEFT OUTER JOIN MTB_Products_SubFamily AS ps
            ON ps.[Code]=est.[Code_SubFamilia] AND ps.[Code_Family] = est.[Code_Familia]  
            LEFT OUTER JOIN MTB_Array AS meses
            ON meses.[Code]=est.[Mes] AND meses.Type='mes' 
            WHERE 1=1
            AND est.Delete_Date IS NULL 
            AND est.[Code_Agent] = '$sCodeAgent'
            AND est.[Code_Account] = '$sCodeAccount'
            $sWHEREAnd
            $sGROUPBY
        ) AS Sub
        WHERE AutoId IN $sSQLIn
        $sORDERBY";
        
        $arRow = $this->oDB->query($sSQL);
        //bug($this->_arDinamicLabels);
        return $arRow;                
    }


    
    public function set_code($value){ $this->_code = $value; }
    public function set_code_account($value){ $this->_code_account = $value; }
    public function set_code_agent($value){ $this->_code_agent = $value; }
    public function set_code_erp($value){ $this->_code_erp = $value; }
    public function set_code_familia($value){ $this->_code_familia = $value; }
    public function set_code_subfamilia($value){ $this->_code_subfamilia = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_empresa($value){ $this->_empresa = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_mes($value){ $this->_mes = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_ventasact($value){ $this->_ventasact = $value; }
    public function set_ventasant($value){ $this->_ventasant = $value; }
    
    public function get_code(){ return $this->_code; }
    public function get_code_account(){ return $this->_code_account; }
    public function get_code_agent(){ return $this->_code_agent; }
    public function get_code_erp(){ return $this->_code_erp; }
    public function get_code_familia(){ return $this->_code_familia; }
    public function get_code_subfamilia(){ return $this->_code_subfamilia; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_empresa(){ return $this->_empresa; }
    public function get_id(){ return $this->_id; }
    public function get_mes(){ return $this->_mes; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_ventasact(){ return $this->_ventasact; }
    public function get_ventasant(){ return $this->_ventasant; }   
    
    public function get_meses(){ return $this->_arMeses; }
    public function set_meses($value){ $this->_arMeses = $value;}
    
    public function get_dinamic_labels(){return $this->_arDinamicLabels;}
    public function set_dinamic_labels($arLabels){$this->_arDinamicLabels=$arLabels;}
    
    public function get_params_for_list(){return $this->_arParamsList;}
    //public function set_dinamic_labels($arParams){$this->_arParamsList=$arParams;}
}
