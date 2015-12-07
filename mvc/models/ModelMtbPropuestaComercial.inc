<?php
class ModelMtbPropuestaComercial extends MainModel implements InterfaceModel
{
    protected $_code;
    private $_code_agent;
    private $_code_articulo;
    private $_code_cliente;
    private $_code_familia;
    private $_code_subfamilia;
    private $_concepto;
    private $_descripcion;
    private $_dto;
    private $_enviado_a_erp;
    private $_estado;
    private $_fechar;
    private $_fechas;
    private $_is_neto;
    private $_valor_old;
    
    private $_id;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    private $_modify_date;
    private $_modify_user;
    
    //Para detalle
    private $_descripcion_familia;
    private $_descripcion_subfamilia;
    private $_descripcion_producto;
    private $_typeof_new;
    
    
    public function __construct($code_user_session="",$code="", 
        $code_agent="", $code_articulo="", $code_cliente="", $code_familia="",
        $code_subfamilia="", $concepto="", $descripcion="", $dto="", $enviado_a_erp="",
        $estado="", $fechar="", $fechas="", $is_neto="", $valor_old="",
        $create_date="", $create_user="", $modify_user="", $modify_date="", 
        $delete_date="", $delete_user="", $id="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "MTB_Propuesta_Comercial";
        $this->_table_code = "";
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT  Code, Code_Cliente, Code_Familia, Code_SubFamilia, Dto, Code_Agent, 
                Code_Articulo, Is_Neto, Descripcion, Estado, FechaS, Enviado_a_Erp, FechaR, Concepto, Valor_Old,
		Id, Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date
                FROM MTB_Propuesta_Comercial";
        $this->_default_sql = $sSQL;

        $this->_code = $code;
        $this->_code_agent = $code_agent;
        $this->_code_articulo = $code_articulo;
        $this->_code_cliente = $code_cliente;
        $this->_code_familia = $code_familia;
        $this->_code_subfamilia = $code_subfamilia;
        $this->_concepto = $concepto;
        $this->_descripcion = $descripcion;
        $this->_dto = $dto;
        $this->_enviado_a_erp = $enviado_a_erp;
        $this->_estado = $estado;
        $this->_fechar = $fechar;
        $this->_fechas = $fechas;
        $this->_is_neto = $is_neto;
        $this->_valor_old = $valor_old;

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
        $this->_code = $arRow["Code"];
        $this->_code_agent = $arRow["Code_Agent"];
        $this->_code_articulo = $arRow["Code_Articulo"];
        $this->_code_cliente = $arRow["Code_Cliente"];
        $this->_code_familia = $arRow["Code_Familia"];
        $this->_code_subfamilia = $arRow["Code_SubFamilia"];
        $this->_concepto = $arRow["Concepto"];
        $this->_descripcion = $arRow["Descripcion"];
        $this->_dto = $arRow["Dto"];
        $this->_enviado_a_erp = $arRow["Enviado_a_Erp"];
        $this->_estado = $arRow["Estado"];
        $this->_fechar = $arRow["FechaR"];
        $this->_fechas = $arRow["FechaS"];
        $this->_is_neto = $arRow["Is_Neto"];
        $this->_valor_old = $arRow["Valor_Old"];

        $this->_id = $arRow["Id"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];
    }

    public function get_mobile_list_by_account()
    {
        $sCodeCliente = $this->_code_cliente;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        //Crear condición
        $arValues["p.Code_Articulo"] = $this->_code_articulo;
        $arValues["pr.Descripcion"] = $this->_descripcion_producto;

        $sCondSearch = $this->build_sql_like_for_search($arValues);
        $sCondSearch = trim($sCondSearch);//Por los \n
        if(!empty($sCondSearch)) $sCondSearch = "AND $sCondSearch";        
        if(!empty($this->_concepto)) $sCondSearch .="AND p.Concepto='$this->_concepto'\n";
        if(!empty($this->_code_familia)) $sCondSearch .="AND p.Code_Familia='$this->_code_familia'\n";
        if(!empty($this->_code_subfamilia)) $sCondSearch .="AND p.Code_SubFamilia='$this->_code_subfamilia'\n";
        
        $sSQL = "
        SELECT p.Id AS Id
        FROM MTB_Propuesta_Comercial AS p
        LEFT JOIN core_users AS u
        ON p.Code_Agent = u.Code
        LEFT JOIN clients AS c
        ON p.Code_Cliente = c.Code
        LEFT JOIN MTB_Products_Family AS prf
        ON p.Code_Familia = prf.Code
        LEFT JOIN MTB_Products_SubFamily AS prs
        ON p.Code_Familia = prs.Code_Family AND p.Code_SubFamilia=prs.Code
        LEFT JOIN MTB_Array AS mtbar
        ON p.Estado = mtbar.Code AND mtbar.Type='estado prop'
        LEFT JOIN MTB_Productos AS pr
        ON p.Code_Articulo=pr.Code_Producto
        WHERE 1=1
        AND p.Code_Cliente='$sCodeCliente'\n";

        if(!empty($sCondSearch)) $sSQL .= $sCondSearch;
        
        //bug($sSQL); DIE;
        $arIdPaginate = $this->oDB->query($sSQL);
        
        //bug($arIdPaginate,$sSQL);
        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores autonumericos de la columna Id
        $arIdPaginate = $this->get_field_values($arIdPaginate, "Id");
        
        $sSQL = "
        SELECT p.Id, p.Code, p.Concepto, 
        p.Code_Agent, u.Description AS Comercial,
        p.Code_Cliente, c.Name AS Cliente, 
        p.Code_Familia, prf.Description AS Familia,
        p.Code_SubFamilia, prs.Description AS Subfamilia,
        p.Code_Articulo, pr.Descripcion AS Producto,
        prod_subf_fam = 
        CASE 
            -- Si no hay articulo se comprueba subf y fam
            WHEN (p.Code_Articulo IS NULL) THEN
                CASE 
                    -- Si no hay subfamilia
                    WHEN (p.Code_SubFamilia IS NULL) THEN
                        CASE 
                            -- Si fam, sub y prod es NULL hay un error
                            WHEN (p.Code_Familia IS NULL) THEN 
                                'ERROR'
                            ELSE
                                --familia
                                prf.Description
                        END-- fin Code_Familia
                    -- Code_SubFamilia no nulo
                    ELSE
                        -- subfamilia
                        prs.Description
                END --fin Code_SubFamilia
            ELSE 
                -- producto
                pr.Descripcion
        END,
        p.Estado, mtbar.Description AS Estado_Description,
        p.FechaS,
        p.Dto,
        p.Valor_Old,
        SUBSTRING(p.FechaS,7,2)+'/'+SUBSTRING(p.FechaS,5,2)+'/'+SUBSTRING(p.FechaS,1,4) AS Fecha_Solicitud,
        p.FechaR,
        SUBSTRING(p.FechaR,7,2)+'/'+SUBSTRING(p.FechaR,5,2)+'/'+SUBSTRING(p.FechaR,1,4) AS Fecha_Respuesta,
        p.Descripcion, 
        p.Enviado_a_Erp
        
        FROM MTB_Propuesta_Comercial AS p
        LEFT JOIN core_users AS u
        ON p.Code_Agent = u.Code
        LEFT JOIN clients AS c
        ON p.Code_Cliente = c.Code
        LEFT JOIN MTB_Products_Family AS prf
        ON p.Code_Familia = prf.Code
        LEFT JOIN MTB_Products_SubFamily AS prs
        ON p.Code_Familia = prs.Code_Family AND p.Code_SubFamilia=prs.Code
        LEFT JOIN MTB_Array AS mtbar
        ON p.Estado = mtbar.Code AND mtbar.Type='estado prop'
        LEFT JOIN MTB_Productos AS pr
        ON p.Code_Articulo=pr.Code_Producto
        WHERE 1=1
        AND p.Id IN ".$this->build_sqlin($arIdPaginate,true);
        //bug($arIdPaginate,$sSQL); die;
        $arRow = $this->oDB->query($sSQL);
        //bug($arRow,$sSQL); 
        return $arRow;                
    }

    public function get_new_autonum_key($sFieldName="Code")
    {
        return $this->get_max_numeric_plus($sFieldName);
    }
    
    public function set_code($value){ $this->_code = $value; }
    public function set_code_agent($value){ $this->_code_agent = $value; }
    public function set_code_articulo($value){ $this->_code_articulo = $value; }
    public function set_code_cliente($value){ $this->_code_cliente = $value; }
    public function set_code_familia($value){ $this->_code_familia = $value; }
    public function set_code_subfamilia($value){ $this->_code_subfamilia = $value; }
    public function set_concepto($value){ $this->_concepto = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_descripcion($value){ $this->_descripcion = $value; }
    public function set_dto($value){ $this->_dto = $value; }
    public function set_enviado_a_erp($value){ $this->_enviado_a_erp = $value; }
    public function set_estado($value){ $this->_estado = $value; }
    public function set_fechar($value){ $this->_fechar = $value; }
    public function set_fechas($value){ $this->_fechas = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_is_neto($value){ $this->_is_neto = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_valor_old($value){ $this->_valor_old = $value; }

    public function get_code(){ return $this->_code; }
    public function get_code_agent(){ return $this->_code_agent; }
    public function get_code_articulo(){ return $this->_code_articulo; }
    public function get_code_cliente(){ return $this->_code_cliente; }
    public function get_code_familia(){ return $this->_code_familia; }
    public function get_code_subfamilia(){ return $this->_code_subfamilia; }
    public function get_concepto(){ return $this->_concepto; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_descripcion(){ return $this->_descripcion; }
    public function get_dto(){ return $this->_dto; }
    public function get_enviado_a_erp(){ return $this->_enviado_a_erp; }
    public function get_estado(){ return $this->_estado; }
    public function get_fechar(){ return $this->_fechar; }
    public function get_fechas(){ return $this->_fechas; }
    public function get_id(){ return $this->_id; }
    public function get_is_neto(){ return $this->_is_neto; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_valor_old(){ return $this->_valor_old; }
    
    //Para formulario detalle
    public function get_descripcion_familia(){ return $this->_descripcion_familia; }
    public function get_descripcion_subfamilia(){ return $this->_descripcion_subfamilia; }
    public function get_descripcion_producto(){ return $this->_descripcion_producto; }
    public function get_typeof_new(){ return $this->_typeof_new; }
    
    public function set_descripcion_familia($value){ $this->_descripcion_familia = $value; }
    public function set_descripcion_subfamilia($value){ $this->_descripcion_subfamilia = $value; }
    public function set_descripcion_producto($value){ $this->_descripcion_producto = $value; }
    public function set_typeof_new($value){ $this->_typeof_new = $value; }
}
