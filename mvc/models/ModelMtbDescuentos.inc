<?php
class ModelMtbDescuentos extends MainModel implements InterfaceModel
{
    protected $_code;
    private $_cantidad;
    private $_code_cliente;
    private $_code_articulo;
    private $_code_familia;
    private $_code_subfamilia;
    private $_dto1;
    private $_is_neto;
    private $_precio;
    
    private $_id;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    private $_modify_date;
    private $_modify_user;

    //private $_descripcion_familia;
    private $_descripcion_subfamilia;
    private $_descripcion_producto;
    
    public function __construct($code_user_session="",$code="", $code_cliente="",
             $code_articulo="", $code_familia="", $code_subfamilia="",
            $cantidad="", $dto1="", $is_neto="", $precio="", $create_date="", $create_user="", 
            $modify_user="", $modify_date="", $delete_date="", $delete_user="", $id="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "MTB_Descuentos";
        $this->_table_code = "";
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT Code, Code_Cliente, Code_Familia, Code_SubFamilia, Code_Articulo, Cantidad, Precio, Dto1, Is_Neto, 
                    Id, Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date
                 FROM MTB_Descuentos";
        $this->_default_sql = $sSQL;

        $this->_code = $code;
        $this->_cantidad = $cantidad;

        $this->_code_articulo = $code_articulo;
        $this->_code_cliente = $code_cliente;
        $this->_code_familia = $code_familia;
        $this->_code_subfamilia = $code_subfamilia;
        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_delete_date = $delete_date;
        $this->_delete_user = $delete_user;
        $this->_dto1 = $dto1;
        $this->_id = $id;
        $this->_is_neto = $is_neto;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_precio = $precio;
    }

    public function update(){}
    public function delete(){}
    public function insert(){}
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arRow["Code"];
        $this->_cantidad = $arRow["Cantidad"];
        $this->_code_articulo = $arRow["Code_Articulo"];
        $this->_code_cliente = $arRow["Code_Cliente"];
        $this->_code_familia = $arRow["Code_Familia"];
        $this->_code_subfamilia = $arRow["Code_SubFamilia"];
        $this->_dto1 = $arRow["Dto1"];
        $this->_is_neto = $arRow["Is_Neto"];
        $this->_precio = $arRow["Precio"];
        $this->_id = $arRow["Id"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];
    }

    /**
     * Familias de los productos que empiezan con $this->_code_articulo
     * del cliente $sCodeCliente en la tabla MTB_Descuentos
     * @param string $sCodeCliente
     * @return string 
     */
    private function sch_sqland_producto()
    {
        $sSQL = "";
        if(!empty($this->_code_articulo))
            $sSQL = "AND Code_Familia IN (SELECT Code_Familia FROM MTB_Productos WHERE Code_Producto LIKE '$this->_code_articulo%') ";
        return $sSQL;
    }
    
    private function sch_sqland_subfamilia()
    {
        $sSQL = "";
        if(!empty($this->_code_subfamilia))
            $sSQL = "AND Code_Familia IN (SELECT Code_Family FROM MTB_Products_Subfamily WHERE Code='$this->_code_subfamilia') ";
        return $sSQL;        
    }
    
    private function sch_sqland_familia()
    {
        $sSQL = "";
        if(!empty($this->_code_familia))
            $sSQL = "AND Code_Familia='$this->_code_familia' ";
        return $sSQL;        
    }    
    
    private function get_sql_clients_hierarchy($sUserCode)
    {
        $sSQL = "";
        if(!empty($sUserCode))
            $sSQL = "SELECT Code 
                     FROM Clients 
                     WHERE Propietario IN (SELECT Code FROM get_code_vendedores('$sUserCode'))
                    ";
        return $sSQL;             
    }
    
    /*Este listado muestra todos los descuentos con los que
     *cuenta cada una de las familias existentes.
     */
    public function get_mobile_list_family_by_account()
    {
        $sCodeCliente = $this->_code_cliente;
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;

        $sCondSearch .= $this->sch_sqland_producto(); 
        $sCondSearch .= $this->sch_sqland_subfamilia();
        $sCondSearch .= $this->sch_sqland_familia();
        
        $sSQL = "
        SELECT Sub.Code_Familia AS Code_Familia,
        Familia.Description AS Familia_Description
        FROM 
        (
            /*TODAS LAS FAMILIAS CON DESCUENTOS PARA EL CLIENTE $sCodeCliente*/
            SELECT  D1.Code_Familia 
            FROM MTB_Descuentos AS D1
            WHERE 1=1 
            AND D1.Code_Cliente='$sCodeCliente'
        
            UNION 
            
            /*FAMILIAS RESTANTES QUE EXISTEN PERO QUE EL CLIENTE NO TIENE DESCUENTOS*/
            SELECT Code AS Code_Familia
            FROM MTB_Products_Family 
            WHERE Code NOT IN 
            (   
                SELECT DISTINCT Code_Familia 
                FROM MTB_Descuentos WHERE 1=1 
                AND Code_Cliente='$sCodeCliente'
            )

        ) AS Sub
        --Para sacar la descripcion
        INNER JOIN MTB_Products_Family AS Familia
        ON Sub.Code_Familia = Familia.Code
        WHERE 1=1 \n
        ";
        
        if(!empty($sCondSearch))
            $sSQL .= "$sCondSearch\n";
        
        $sSQL .= "GROUP BY Sub.Code_Familia, Familia.Description\n";
        $sSQL .= "ORDER BY Familia.Description ASC";
        //bug($sSQL); die;
        $arPagingCodes = $this->oDB->query($sSQL);
        
        //bug($arPagingCodes,$sSQL); //die;
        $this->_list_total_regs = count($arPagingCodes);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arPagingCodes,$iPage,"",$iItemsPerPage);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arPagingCodes = $oPaginate->get_array_items();
        //bug($arPagingCodes); die;
        //Obtengo los valores distintos que actuan como contador "Code_Familia"
        $arPagingCodes = $this->get_field_values($arPagingCodes, "Code_Familia");
        //bug($arPagingCodes); die;
        $sSQL = "
        SELECT '$sCodeCliente' AS Code_Account, Sub.Code_Familia AS Code_Familia,
		Familia.Description AS Familia_Description,
		Sub.DtoFamilia AS DtoFamilia,
		Sub.HasSub AS DtoSubfamilia,
		Sub.HasNeto AS Is_Neto
        FROM 
        (
            /*TODAS LAS FAMILIAS CON DESCUENTOS PARA EL CLIENTE $sCodeCliente*/
            SELECT  D1.Code_Familia, 
            DtoFamilia =(SELECT MAX(D2.Dto1) 
                         FROM MTB_Descuentos AS D2 
                         WHERE D2.Code_Familia=D1.Code_Familia
                         AND D2.Code_Cliente='$sCodeCliente'
                        ),
            HasSub=(CASE 
                        WHEN
                            (SELECT COUNT(DISTINCT D2.Code_SubFamilia) 
                             FROM MTB_Descuentos AS D2 
                             WHERE D2.Code_SubFamilia IS NOT NULL
                             AND D2.Code_Familia = D1.Code_Familia
                             AND D2.Code_Articulo IS NULL
                             AND D2.Dto1>0
                             AND D2.Code_Cliente='$sCodeCliente'
                            )>0
                        THEN 'SI'
                        ELSE 'NO'
                     END),
            HasNeto=(CASE 
                        WHEN
                            (SELECT COUNT(DISTINCT D2.Code_Articulo) 
                             FROM MTB_Descuentos AS D2 
                             WHERE D2.Code_SubFamilia IS NOT NULL
                             AND D2.Code_Familia = D1.Code_Familia
                             --AND (D2.Code_Articulo IN (SELECT Code_Producto FROM MTB_Productos WHERE Estado='1'))
                             AND D2.Precio>0
                             AND D2.Code_Cliente='$sCodeCliente'
                            )>0
                        THEN 'SI'
                        ELSE 'NO'
                    END)
            FROM MTB_Descuentos AS D1
            WHERE 1=1 
            AND D1.Code_Cliente='$sCodeCliente'
        
            UNION 
            
            /*FAMILIAS RESTANTES QUE EXISTEN PERO QUE EL CLIENTE NO TIENE DESCUENTOS*/
            SELECT Code AS Code_Familia, 0.00 AS DtoFamilia, 'NO' AS HasSub, 'NO' AS HasNeto
            FROM MTB_Products_Family 
            WHERE Code NOT IN 
            (   
                SELECT DISTINCT Code_Familia 
                FROM MTB_Descuentos WHERE 1=1 
                AND Code_Cliente='$sCodeCliente'
            )

        ) AS Sub
        INNER JOIN MTB_Products_Family AS Familia
        ON Sub.Code_Familia = Familia.Code
        WHERE 1=1\n
        ";        
        $sSQL .= "AND Sub.Code_Familia IN ".$this->build_sqlin($arPagingCodes)."\n";
        $sSQL .= "GROUP BY Sub.Code_Familia, Familia.Description, Sub.DtoFamilia, Sub.HasSub, Sub.HasNeto\n";
        $sSQL .= "ORDER BY Familia.Description ASC";
        //bug($arIdPaginate,$sSQL); die;
        $arRow = $this->oDB->query($sSQL);
        //bug($arRow,$sSQL); die;
        return $arRow;                
    }

    public function get_mobile_list_subfamily_by_account()
    {
        $sCodeCliente = $this->_code_cliente;
        $sCodeFamilia = $this->_code_familia;
        
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
 
        $arValues["Subfamilia_Description"] = $this->_descripcion_subfamilia;
        $sCondSearch = $this->build_sql_like_for_search($arValues);

        //bug($sCondSearch);
        $sSQL = "
        SELECT Code_SubFamilia
        FROM 
        (
            /*todas las subfamilias con descuentos que tiene el cliente x de la familia z*/
            SELECT --D1.Code, 
            D1.Code_Cliente AS Code_Account, 
            D1.Code_Familia, pf.Description AS Familia_Description, 
            D1.Code_SubFamilia,ps.Description AS Subfamilia_Description, 
            --D1.Code_Articulo, D1.Cantidad, D1.Precio, D1.Is_Neto
            D1.Dto1
            FROM MTB_Descuentos AS D1
            LEFT JOIN MTB_Products_Family AS pf
            ON D1.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON D1.Code_Familia = ps.Code_Family
            AND D1.Code_SubFamilia = ps.Code
            WHERE 1=1
            AND D1.Code_Cliente='$sCodeCliente'
            AND D1.Code_Familia='$sCodeFamilia'
            --Indica que no se incluyan los descuentos por familia
            AND (Code_SubFamilia IS NOT NULL AND LTRIM(RTRIM(Code_SubFamilia))!='')
            --Indica que no se incluyan los descuentos por articulo
            AND (Code_Articulo IS NULL OR LTRIM(RTRIM(Code_Articulo))='') 
            --AND Code_Familia IS NOT NULL AND Code_Familia!=''

            UNION

            --Todas las subfamilias restantes de la familia z que no tienen descuentos
            SELECT '$sCodeCliente' AS Code_Account,
            ps.Code_Family AS Code_Familia, pf.Description AS Familia_Description,
            ps.Code AS Code_SubFamilia, ps.Description AS Subfamilia_Description,
            0.00 AS Dto1
            FROM MTB_Products_SubFamily AS ps
            LEFT JOIN MTB_Products_Family AS pf
            ON ps.Code_Family = pf.Code
            WHERE 1=1
            AND ps.Code_Family = '$sCodeFamilia'
            AND ps.Code NOT IN
            (
                    -- Las subfamilias en descuentos
                    SELECT 
                    --D1.Code, D1.Code_Cliente, D1.Code_Familia, 
                    D1.Code_SubFamilia
                    --, D1.Code_Articulo, D1.Cantidad, 
                    --D1.Precio, D1.Dto1, D1.Is_Neto
                    FROM MTB_Descuentos AS D1
                    WHERE 1=1
                    AND D1.Code_Cliente='$sCodeCliente'
                    AND D1.Code_Familia='$sCodeFamilia'
                    --Indica que no se incluyan los descuentos por familia
                    AND (Code_SubFamilia IS NOT NULL AND LTRIM(RTRIM(Code_SubFamilia))!='')
                    --Indica que no se incluyan los descuentos por articulo
                    AND (Code_Articulo IS NULL OR LTRIM(RTRIM(Code_Articulo))='') 
            )
        ) AS Sub
        WHERE 1=1 
        ";
        
        if(!empty($sCondSearch)) 
            $sSQL .= "AND $sCondSearch\n";
        $sSQL .= "ORDER BY Subfamilia_Description ASC";
        //bug($sSQL); die;
        $arPagingCodes = $this->oDB->query($sSQL);
        
        //bug($arPagingCodes,$sSQL); die;
        $this->_list_total_regs = count($arPagingCodes);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arPagingCodes,$iPage,"",$iItemsPerPage);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arPagingCodes = $oPaginate->get_array_items();
        //bug($arPagingCodes); die;
        //Obtengo los valores distintos que actuan como contador "Code_Familia"
        $arPagingCodes = $this->get_field_values($arPagingCodes, "Code_SubFamilia");
        //bug($arPagingCodes); die;
        $sSQL = "
        SELECT Code_Account, 
        Code_Familia, Familia_Description,
        Code_SubFamilia, Subfamilia_Description,
        Dto1,
        Type='subfamilia' --Necesario para crear el link desde el listado
        FROM 
        (
            /*todas las subfamilias con descuentos que tiene el cliente x de la familia z*/
            SELECT --D1.Code, 
            D1.Code_Cliente AS Code_Account, 
            D1.Code_Familia, pf.Description AS Familia_Description, 
            D1.Code_SubFamilia,ps.Description AS Subfamilia_Description, 
            --D1.Code_Articulo, D1.Cantidad, D1.Precio, D1.Is_Neto
            D1.Dto1
            FROM MTB_Descuentos AS D1
            LEFT JOIN MTB_Products_Family AS pf
            ON D1.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON D1.Code_Familia = ps.Code_Family
            AND D1.Code_SubFamilia = ps.Code
            WHERE 1=1
            AND D1.Code_Cliente='$sCodeCliente'
            AND D1.Code_Familia='$sCodeFamilia'
            --Indica que no se incluyan los descuentos por familia
            AND (Code_SubFamilia IS NOT NULL AND LTRIM(RTRIM(Code_SubFamilia))!='')
            --Indica que no se incluyan los descuentos por articulo
            AND (Code_Articulo IS NULL OR LTRIM(RTRIM(Code_Articulo))='') 
            --AND Code_Familia IS NOT NULL AND Code_Familia!=''

            UNION

            --Todas las subfamilias restantes de la familia z que no tienen descuentos
            SELECT '$sCodeCliente' AS Code_Account,
            ps.Code_Family AS Code_Familia, pf.Description AS Familia_Description,
            ps.Code AS Code_SubFamilia,  ps.Description AS Subfamilia_Description,
            0.00 AS Dto1
            FROM MTB_Products_SubFamily AS ps
            LEFT JOIN MTB_Products_Family AS pf
            ON ps.Code_Family = pf.Code
            WHERE 1=1
            AND ps.Code_Family = '$sCodeFamilia'
            AND ps.Code NOT IN
            (
                    -- Las subfamilias en descuentos
                    SELECT 
                    --D1.Code, D1.Code_Cliente, D1.Code_Familia, 
                    D1.Code_SubFamilia
                    --, D1.Code_Articulo, D1.Cantidad, 
                    --D1.Precio, D1.Dto1, D1.Is_Neto
                    FROM MTB_Descuentos AS D1
                    WHERE 1=1
                    AND D1.Code_Cliente='$sCodeCliente'
                    AND D1.Code_Familia='$sCodeFamilia'
                    --Indica que no se incluyan los descuentos por familia
                    AND (Code_SubFamilia IS NOT NULL)
                    --Indica que no se incluyan los descuentos por articulo
                    AND (Code_Articulo IS NULL OR LTRIM(RTRIM(Code_Articulo))='') 
            )
        ) AS Sub
        WHERE 1=1
        ";        
        $sSQL .= "AND Code_SubFamilia IN ".$this->build_sqlin($arPagingCodes)."\n";
        $sSQL .= "ORDER BY Subfamilia_Description ASC";
        //bug($arPagingCodes,$sSQL); die;
        $arRow = $this->oDB->query($sSQL);
        //bug($arRow,$sSQL); die;
        return $arRow;                
    }
    
    public function get_mobile_list_neto_by_account()
    {
        $sCodeCliente = $this->_code_cliente;
        $sCodeFamilia = $this->_code_familia;
        //$sCodeSubfamilia = $this->_code_subfamilia;
        
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
 
        $arValues["Code_Articulo"] = $this->_code_articulo;
        $arValues["Producto_Description"] = $this->_descripcion_producto;
        $sCondSearch = $this->build_sql_like_for_search($arValues);

        //bug($sCondSearch);
        $sSQL = "
        SELECT Id --, Code_Articulo, Producto_Description
        --Code_Familia, Familia_Description,
        --Code_SubFamilia, Subfamilia_Description,
        --Cantidad, Precio
        FROM 
        (
            /*todos los productos con descuentos que tiene el cliente x */
            SELECT --D1.Code, 
            Id = LTRIM(RTRIM(D1.Code_Familia))+'-'+LTRIM(RTRIM(D1.Code_SubFamilia))+'-'+LTRIM(RTRIM(D1.Code_Articulo)),
            D1.Code_Articulo, p.Descripcion AS Producto_Description,   
            --D1.Code_Cliente AS Code_Account, 
            --D1.Code_Familia, pf.Description AS Familia_Description, 
            D1.Code_SubFamilia, ps.Description AS Subfamilia_Description 
            --D1.Cantidad, D1.Precio
            FROM MTB_Descuentos AS D1
            LEFT JOIN MTB_Products_Family AS pf
            ON D1.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON D1.Code_Familia = ps.Code_Family
            AND D1.Code_SubFamilia = ps.Code
            LEFT JOIN MTB_Productos AS p
            ON D1.Code_Articulo = p.Code_Producto
            AND D1.Code_Familia = p.Code_Familia
            AND D1.Code_SubFamilia = p.Code_SubFamilia
            WHERE 1=1
            AND D1.Code_Cliente='$sCodeCliente'
            AND D1.Code_Familia='$sCodeFamilia'
            --Indica que es un descuento neto
            AND (Code_Articulo IS NOT NULL) 

            UNION

            --Todos los productos restantes que no tienen descuentos de la familia y subfamilia inidicada
            SELECT 
            Id = LTRIM(RTRIM(p.Code_Familia))+'-'+LTRIM(RTRIM(p.Code_SubFamilia))+'-'+LTRIM(RTRIM(p.Code_Producto)),
            p.Code_Producto AS Code_Articulo, p.Descripcion AS Producto_Description,
            --'$sCodeCliente' AS Code_Account,
            --p.Code_Familia, pf.Description AS Familia_Description,
            p.Code_SubFamilia, ps.Description AS Subfamilia_Description
            --0 AS Cantidad, 0.00 AS Precio
            FROM MTB_Productos AS p
            LEFT JOIN MTB_Products_Family AS pf
            ON p.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON p.Code_SubFamilia = ps.Code
            AND p.Code_Familia = ps.Code_Family
            WHERE 1=1
            AND p.Code_Producto NOT IN
            (
                -- los productos en descuentos
                SELECT D1.Code_Articulo
                FROM MTB_Descuentos AS D1
                WHERE 1=1
                AND D1.Code_Cliente='$sCodeCliente'
                AND D1.Code_Familia='$sCodeFamilia'
                --Indica que es un descuento neto
                AND (Code_Articulo IS NOT NULL) 
            )
            AND p.Code_Familia = '$sCodeFamilia'
        ) AS Sub
        WHERE 1=1
        ";
        if(!empty($sCondSearch)) $sSQL .= "AND $sCondSearch\n";
        $sSQL .= "ORDER BY Subfamilia_Description, Producto_Description ASC";
        //bug($sSQL); die;
        $arPagingCodes = $this->oDB->query($sSQL);
        
        //bug($arPagingCodes,$sSQL); die;
        $this->_list_total_regs = count($arPagingCodes);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arPagingCodes,$iPage,"",$iItemsPerPage);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arPagingCodes = $oPaginate->get_array_items();
        //bug($arPagingCodes); die;
        //Obtengo los valores distintos que actuan como contador "Code_Familia"
        $arPagingCodes = $this->get_field_values($arPagingCodes, "Id");
        //bug($arPagingCodes); die;
        $sSQL = "
        SELECT Id, Code_Familia, Familia_Description,
        Code_SubFamilia, Subfamilia_Description,
        Code_Articulo, Producto_Description,
        Cantidad, Precio,
        Type='neto',
        Code_Account='$sCodeCliente'
        FROM 
        (
            /*todos los productos con descuentos que tiene el cliente x */
            SELECT --D1.Code, 
            Id = LTRIM(RTRIM(D1.Code_Familia))+'-'+LTRIM(RTRIM(D1.Code_SubFamilia))+'-'+LTRIM(RTRIM(D1.Code_Articulo)),
            D1.Code_Cliente AS Code_Account, 
            D1.Code_Familia, pf.Description AS Familia_Description, 
            D1.Code_SubFamilia, ps.Description AS Subfamilia_Description, 
            D1.Code_Articulo, p.Descripcion AS Producto_Description,
            D1.Cantidad, D1.Precio
            FROM MTB_Descuentos AS D1
            LEFT JOIN MTB_Products_Family AS pf
            ON D1.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON D1.Code_Familia = ps.Code_Family
            AND D1.Code_SubFamilia = ps.Code
            LEFT JOIN MTB_Productos AS p
            ON D1.Code_Articulo = p.Code_Producto
            AND D1.Code_Familia = p.Code_Familia
            AND D1.Code_SubFamilia = p.Code_SubFamilia
            WHERE 1=1
            AND D1.Code_Cliente='$sCodeCliente'
            AND D1.Code_Familia='$sCodeFamilia'
            --Indica que es un descuento neto
            AND (Code_Articulo IS NOT NULL) 

            UNION

            --Todos los productos restantes que no tienen descuentos de la familia y subfamilia inidicada
            SELECT 
            Id = LTRIM(RTRIM(p.Code_Familia))+'-'+LTRIM(RTRIM(p.Code_SubFamilia))+'-'+LTRIM(RTRIM(p.Code_Producto)),
            '$sCodeCliente' AS Code_Account,
            p.Code_Familia, pf.Description AS Familia_Description,
            p.Code_SubFamilia, ps.Description AS Subfamilia_Description,
            p.Code_Producto AS Code_Articulo, p.Descripcion AS Producto_Description,
            0 AS Cantidad, 0.00 AS Precio
            FROM MTB_Productos AS p
            LEFT JOIN MTB_Products_Family AS pf
            ON p.Code_Familia = pf.Code
            LEFT JOIN MTB_Products_SubFamily AS ps
            ON p.Code_SubFamilia = ps.Code
            AND p.Code_Familia = ps.Code_Family
            WHERE 1=1
            AND p.Code_Producto NOT IN
            (
                -- los productos en descuentos
                SELECT D1.Code_Articulo
                FROM MTB_Descuentos AS D1
                WHERE 1=1
                AND D1.Code_Cliente='$sCodeCliente'
                AND D1.Code_Familia='$sCodeFamilia'
                --Indica que es un descuento neto
                AND (Code_Articulo IS NOT NULL) 
            )
            AND p.Code_Familia = '$sCodeFamilia'
        ) AS Sub
        WHERE 1=1 
         ";        
        $sSQL .= "AND Id IN ".$this->build_sqlin($arPagingCodes)."\n";
        $sSQL .= "ORDER BY Subfamilia_Description, Producto_Description ASC";
        //bug($arPagingCodes,$sSQL); die;
        $arRow = $this->oDB->query($sSQL);
        //bug($arRow,$sSQL); die;
        return $arRow;                
        
    }
    
    public function set_cantidad($value){ $this->_cantidad = $value; }
    public function set_code($value){ $this->_code = $value; }
    public function set_code_articulo($value){ $this->_code_articulo = $value; }
    public function set_code_cliente($value){ $this->_code_cliente = $value; }
    public function set_code_familia($value){ $this->_code_familia = $value; }
    public function set_code_subfamilia($value){ $this->_code_subfamilia = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_dto1($value){ $this->_dto1 = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_is_neto($value){ $this->_is_neto = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_precio($value){ $this->_precio = $value; }

    public function get_cantidad(){ return $this->_cantidad; }
    public function get_code(){ return $this->_code; }
    public function get_code_articulo(){ return $this->_code_articulo; }
    public function get_code_cliente(){ return $this->_code_cliente; }
    public function get_code_familia(){ return $this->_code_familia; }
    public function get_code_subfamilia(){ return $this->_code_subfamilia; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_dto1(){ return $this->_dto1; }
    public function get_id(){ return $this->_id; }
    public function get_is_neto(){ return $this->_is_neto; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_precio(){ return $this->_precio; }
    
    //Para filtrado por subfamilia
    public function set_descripcion_subfamilia($value){ $this->_descripcion_subfamilia = $value; }
    public function get_descripcion_subfamilia(){ return $this->_descripcion_subfamilia; }
    
    //Para filtrado por producto
    public function set_descripcion_producto($value){ $this->_descripcion_producto = $value; }
    public function get_descripcion_producto(){ return $this->_descripcion_producto; } 
}