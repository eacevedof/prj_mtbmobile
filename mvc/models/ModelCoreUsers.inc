<?php
class ModelCoreUsers extends MainModel implements InterfaceModel
{
    protected $_code;
    private $_active;
    private $_code_manager;
    private $_code_theme;
    private $_databox;
    private $_delegation;
    private $_desarrollador;
    private $_description;
    private $_email;
    private $_first_name;
    private $_id;
    private $_last_name;
    private $_login;
    private $_password;
    private $_passwordl;
    private $_start_module;
    private $_supplant;
    private $_user_language;
    private $_user_level;
    
    public function __construct($code_user_session,$code, $active="", $code_manager="", $code_theme="",
        $databox="", $delegation="", $desarrollador="", $description="", $email="", $first_name="",
        $id="", $last_name="", $login="", $password="", $passwordl="", $start_module="", $supplant="",
        $user_language="", $user_level="")
    {
        global  $oDB, $oSite;
        //$this->oSite = $oSite;
        $this->oDB = $oDB;
        $this->_table_name = "core_users";
        $this->_table_code = "x1229";
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT Id, Desarrollador, Databox, Delegation, Code, Description, 
                        Login, Password, User_Level, Code_Manager, Active, Start_Module, 
                        User_Language, First_Name, Last_Name, Email, Code_Theme, passwordL                        
                FROM core_users ";
        $this->_default_sql = $sSQL;
        
        $this->_active = $active;
        $this->_code = $code;
        $this->_code_manager = $code_manager;
        $this->_code_theme = $code_theme;
        $this->_databox = $databox;
        $this->_delegation = $delegation;
        $this->_desarrollador = $desarrollador;
        $this->_description = $description;
        $this->_email = $email;
        $this->_first_name = $first_name;
        $this->_id = $id;
        $this->_last_name = $last_name;
        $this->_login = $login;
        $this->_password = $password;
        $this->_passwordl = $passwordl;
        $this->_start_module = $start_module;
        $this->_supplant = $supplant;
        $this->_user_language = $user_language;
        $this->_user_level = $user_level;
    }

    public function update(){}
    
    public function delete(){}
     
    public function insert(){}
    
    protected function set_all_properties($arRow) 
    {
        $this->_active = $arRow["Active"];
        $this->_code = $arRow["Code"];
        $this->_code_manager = $arRow["Code_Manager"];
        $this->_code_theme = $arRow["Code_Theme"];
        $this->_databox = $arRow["Databox"];
        $this->_delegation = $arRow["Delegation"];
        $this->_desarrollador = $arRow["Desarrollador"];
        $this->_description = $arRow["Description"];
        $this->_email = $arRow["Email"];
        $this->_first_name = $arRow["First_Name"];
        $this->_id = $arRow["Id"];
        $this->_last_name = $arRow["Last_Name"];
        $this->_login = $arRow["Login"];
        $this->_password = $arRow["Password"];
        $this->_passwordl = $arRow["passwordL"];
        $this->_start_module = $arRow["Start_Module"];
        $this->_supplant = $arRow["Supplant"];
        $this->_user_language = $arRow["User_Language"];
        $this->_user_level = $arRow["User_Level"];
    }

    public function get_mobile_list()
    {
        
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;

        $arValues["c.name"] = $this->_name;
        $arValues["c.codigo_sic"] = $this->_codigo_sic;
        $arValues["cp.Cp"] = $this->_cp;
        $arValues["mtc.Poblacion"] = $this->_mst_poblacion;
        $sCondSearch = $this->build_sql_like_for_search($arValues);
        
        $sSQL = "SELECT c.Id
                FROM clients AS c
                INNER JOIN MTB_Clientes AS mtc
                ON c.Code = mtc.Code
                LEFT JOIN core_users u
                ON c.propietario = u.code
                -- PICKLIST
                LEFT JOIN MTB_Codigos_Postales AS cp
                ON c.Cp = cp.Code
                WHERE 1=1 
                -- AND 1=2
                AND c.Propietario IN (SELECT Code FROM get_code_vendedores('$sCodeUserSession'))";
        
        if(!empty($sCondSearch))
            $sSQL .= " AND $sCondSearch";
        
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
        
        $sSQL = "SELECT c.Code, c.Name AS Nombre, mtc.Poblacion AS Poblacion, 
                cp.Code AS cp_code, cp.Cp, c.Propietario AS Code_User, 
                u.Description AS Vendedor
                FROM clients AS c
                INNER JOIN MTB_Clientes AS mtc
                ON c.Code = mtc.Code
                LEFT JOIN core_users u
                ON c.propietario = u.code
                -- PICKLIST
                LEFT JOIN MTB_Codigos_Postales AS cp
                ON c.Cp = cp.Code
                WHERE 1=1 
                -- AND 1=2
                AND c.Id IN ".$this->build_sqlin($arIdPaginate,true);
        //bug($arIdPaginate,$sSQL); die;
        $arRow = $this->oDB->query($sSQL);
        //bug($arRow,$sSQL); 
        return $arRow;                
    }

    public function get_code(){ return $this->_code; }
    public function is_active(){ return $this->_active; }
    public function get_code_manager(){ return $this->_code_manager; }
    public function get_code_theme(){ return $this->_code_theme; }
    public function get_databox(){ return $this->_databox; }
    public function get_delegation(){ return $this->_delegation; }
    public function get_desarrollador(){ return $this->_desarrollador; }
    public function get_description(){ return $this->_description; }
    public function get_email(){ return $this->_email; }
    public function get_first_name(){ return $this->_first_name; }
    public function get_id(){ return $this->_id; }
    public function get_last_name(){ return $this->_last_name; }
    public function get_login(){ return $this->_login; }
    public function get_password(){ return $this->_password; }
    public function get_passwordl(){ return $this->_passwordl; }
    public function get_start_module(){ return $this->_start_module; }
    public function get_supplant(){ return $this->_supplant; }
    public function get_user_language(){ return $this->_user_language; }
    public function get_user_level(){ return $this->_user_level; }
    
    public function set_active($value){ $this->_active = $value; }
    public function set_code($value){ $this->_code = $value; }
    public function set_code_manager($value){ $this->_code_manager = $value; }
    public function set_code_theme($value){ $this->_code_theme = $value; }
    public function set_databox($value){ $this->_databox = $value; }
    public function set_delegation($value){ $this->_delegation = $value; }
    public function set_desarrollador($value){ $this->_desarrollador = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_email($value){ $this->_email = $value; }
    public function set_first_name($value){ $this->_first_name = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_last_name($value){ $this->_last_name = $value; }
    public function set_login($value){ $this->_login = $value; }
    public function set_password($value){ $this->_password = $value; }
    public function set_passwordl($value){ $this->_passwordl = $value; }
    public function set_start_module($value){ $this->_start_module = $value; }
    public function set_supplant($value){ $this->_supplant = $value; }
    public function set_user_language($value){ $this->_user_language = $value; }
    public function set_user_level($value){ $this->_user_level = $value; }
}
