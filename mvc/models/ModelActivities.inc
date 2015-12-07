<?php
class ModelActivities extends MainModel implements InterfaceModel
{
    protected $_code;

    private $_code_user;
    private $_code_account;
    private $_code_contact;
    
    private $_code_newtype;
    private $_code_newtype_next;
    private $_code_result;
    //private $_code_type_issue;
    private $_date_next;
    private $_datep;
    //private $_description;
    private $_end_time;
    private $_id;
    private $_notas_centralita;
    private $_notes;
    private $_start_time;    
    
    private $_modify_date;
    private $_modify_user;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    
    public $oUser; //Vendedor
    public $oClient;
    public $oContact;
    
    private $_contact_description; //Busqueda
    private $_user_description;
    private $_is_key_in_table = false;
    
    
    public function __construct($code_user_session="", $code="", ModelCoreUsers $oVendedor=null,
        ModelClients $oClient=null, ModelContacts $oContact=null, $code_user="",
        $code_account="", $code_contact="", $code_newtype="", $code_newtype_next="",
        $code_result="", $code_type_issue="", $date_next="", $datep="", $description="",
        $end_time="", $id="", $notas_centralita="", $notes="", $start_time="", $create_date="",
        $create_user="", $modify_date="", $modify_user="", $delete_date="", $delete_user=""            
    )
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "activities";
        $this->_table_code = ""; 
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT Code, Delete_User, Delete_Date, Create_User, Create_Date, Modify_User, Modify_Date,   Code_User,
                 Start_Time, DateP, End_Time, Code_Account, Code_Contact, Notas_Centralita, Date_Next, Code_Newtype_Next,
                 Code_Newtype, Code_Result, Notes
                 --, Description, Code_Type_Issue
                FROM activities ";
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->_code_user = $code_user;
        $this->_code_account = $code_account;
        $this->_code_contact = $code_contact;
        
        $this->oUser = new ModelCoreUsers($code_user_session, $code);
        $this->oClient = new ModelClients($code_user_session,$code_account);
        $this->oContact = new ModelContacts($code_user_session,$code_contact);
        
        if(!empty($oVendedor))
        {
            $this->oUser = $oVendedor;
            $this->_code_user = $this->oUser->get_code();
        }
        
        if(!empty($oClient))
        {
            $this->oClient = $oClient; 
            $this->_code_account = $oClient->get_code();
        }

        if(!empty($oContact))
        {
            $this->oContact = $oContact;
            $this->_code_contact = $oContact->get_code();            
        }

        $this->_code_newtype = $code_newtype;
        $this->_code_newtype_next = $code_newtype_next;
        $this->_code_result = $code_result;
        //$this->_code_type_issue = $code_type_issue;
        
        $this->_date_next = $date_next;
        $this->_datep = $datep;
        //$this->_description = $description;
        $this->_end_time = $end_time;
        $this->_id = $id;
        $this->_notas_centralita = $notas_centralita;
        $this->_notes = $notes;
        $this->_start_time = $start_time;

        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_delete_date = $delete_date;
        $this->_delete_user = $delete_user;        
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
    }

    public function load_by_code()
    {
        $sSQL = $this->_default_sql;
        $sSQL .= " WHERE Code='$this->_code'";
        //bug($sSQL,"en load by code"); die;
        $this->load_from_sql($sSQL);
        
        $this->oUser = new ModelCoreUsers(null, $this->_code_user);
        $this->oUser->load_by_code();
        $this->oClient = new ModelClients(null, $this->_code_account);
        $this->oClient->load_by_code();
        //$this->oMtbCliente = new ModelMtbClientes($this->oClient);
        //$this->oMtbCliente->load_by_code();
        $this->oContact = new ModelContacts(null, $this->_code_contact);
        $this->oContact->load_by_code();
    }
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arRow["Code"];
        $this->_code_account = $arRow["Code_Account"];
        $this->_code_contact = $arRow["Code_Contact"];
        $this->_code_newtype = $arRow["Code_Newtype"];
        $this->_code_newtype_next = $arRow["Code_Newtype_Next"];
        $this->_code_result = $arRow["Code_Result"];
        //$this->_code_type_issue = $arRow["Code_Type_Issue"];
        $this->_code_user = $arRow["Code_User"];
        $this->_date_next = $arRow["Date_Next"];
        $this->_datep = $arRow["DateP"];
        //$this->_description = $arRow["Description"];
        $this->_end_time = $arRow["End_Time"];
        $this->_id = $arRow["Id"];
        $this->_notas_centralita = $arRow["Notas_Centralita"];
        $this->_notes = $arRow["Notes"];
        //la longitud del campo es 20 mientras que end time es 2 por eso
        //se crean espacios en blanco
        $this->_start_time = trim($arRow["Start_Time"]);
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];      
    }
    
    public function get_mobile_list_by_account()
    {
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        $sCodeAccount = $this->_code_account;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;
        //Campos de busqueda para construir like
        $arValues["a.DateP"] = $this->_datep;
        $arValues["co.Description"] = $this->_contact_description;
        $sCondSearch = $this->build_sql_like_for_search($arValues);
        
        $sSQL = "SELECT DISTINCT a.Id
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code
                WHERE 1=1
                AND a.Code_Result='0' 
                AND a.Code_Account='$sCodeAccount'
                ";
        
        if(!empty($sCondSearch))
            $sSQL .= " AND $sCondSearch";
        
        $arIdPaginate = $this->oDB->query($sSQL);

        //bug($arIdPaginate,$sSQL);
        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        //Si la página pedida tiene un valor fuera de rango se corrige en el componente y hay que
        //actualizarla
        $this->_list_page = $oPaginate->get_pagina();
        //bug($this->_list_page);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        //bug($this->_list_num_pages);
        //bug($this->_list_arPages);
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores autonumericos de la columna Id
        $arIdPaginate = $this->get_field_values($arIdPaginate, "Id");
        
        $sSQL = "SELECT DISTINCT a.Id, 
                a.Code,
                a.Code_Account, c.Name,
                a.DateP AS FechaHydra, 
                SUBSTRING(a.DateP,7,2)+'/'+SUBSTRING(a.DateP,5,2)+'/'+SUBSTRING(a.DateP,1,4) AS Fecha,
                a.Code_Contact, co.Description AS Contacto,
                a.Code_User, u.Description AS Comercial
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code

                WHERE 1=1
                AND a.Id IN ".$this->build_sqlin($arIdPaginate,true);
        
        $sSQL .= " ORDER BY a.DateP DESC";
        //bug($arIdPaginate,$sSQL); die;
        $arData = $this->oDB->query($sSQL);
        //bug($arData,$sSQL); 
        return $arData;                
    }
    
    public function get_mobile_list_by_user()
    {
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;
        //Campos de busqueda para construir like
        $arValues["a.DateP"] = $this->_datep;
        $arValues["u.Description"] = $this->_user_description;
        $sCondSearch = $this->build_sql_like_for_search($arValues);
        
        $sSQL = "SELECT DISTINCT a.Id
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code
                WHERE 1=1
                AND a.Code_Result='0' 
                AND a.Code_User IN (SELECT Code FROM get_code_vendedores('$sCodeUserSession')) 
                ";
        
        if(!empty($sCondSearch))
            $sSQL .= " AND $sCondSearch";
        
        $arIdPaginate = $this->oDB->query($sSQL);

        //bug($arIdPaginate,$sSQL);
        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        //Si la página pedida tiene un valor fuera de rango se corrige en el componente y hay que
        //actualizarla
        $this->_list_page = $oPaginate->get_pagina();
        //bug($this->_list_page);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores autonumericos de la columna Id
        $arIdPaginate = $this->get_field_values($arIdPaginate, "Id");
        
        $sSQL = "SELECT DISTINCT a.Id, 
                a.Code, c.Name, 
                a.DateP AS FechaHydra, 
                SUBSTRING(a.DateP,7,2)+'/'+SUBSTRING(a.DateP,5,2)+'/'+SUBSTRING(a.DateP,1,4) AS Fecha,
                a.Code_Contact, co.Description AS Contacto,
                a.Code_User, u.Description AS Comercial
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code

                WHERE 1=1
                AND a.Id IN ".$this->build_sqlin($arIdPaginate,true);
        
        $sSQL .= " ORDER BY a.DateP DESC";
        //bug($arIdPaginate,$sSQL); die;
        $arData = $this->oDB->query($sSQL);
        //bug($arData,$sSQL); 
        return $arData;                
    }
    
    public function get_mobile_list_by_history()
    {
        $sCondSearch = "";
        //$sUrl = $this->_list_url;
        $iPage = $this->_list_page;
        $iItemsPerPage = $this->_list_items_per_page;
        $sCodeAccount = $this->_code_account;
        
        $sCodeUserSession = $this->_code_user_session;
        if(empty($sCodeUserSession))
            $sCodeUserSession = -1;
        //Campos de busqueda para construir like
        $arValues["a.DateP"] = $this->_datep;
        $arValues["co.Description"] = $this->_contact_description;
        $sCondSearch = $this->build_sql_like_for_search($arValues);
        
        $sSQL = "SELECT DISTINCT a.Id
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code
                WHERE 1=1
                AND a.Code_Result!='0' 
                AND a.Code_Account='$sCodeAccount'
                ";
        
        if(!empty($sCondSearch))
            $sSQL .= " AND $sCondSearch";
        
        $arIdPaginate = $this->oDB->query($sSQL);

        //bug($arIdPaginate,$sSQL);
        $this->_list_total_regs = count($arIdPaginate);
        //No hace falta la url pq no se usa botones tipo link
        $oPaginate = new ComponentPaginate($arIdPaginate,$iPage,"",$iItemsPerPage);
        //Si la página pedida tiene un valor fuera de rango se corrige en el componente y hay que
        //actualizarla
        $this->_list_page = $oPaginate->get_pagina();
        //bug($this->_list_page);
        $this->_list_num_pages = $oPaginate->get_num_paginas();
        $this->_list_arPages = $oPaginate->get_paginas_for_select();
        //bug($this->_list_num_pages);
        //bug($this->_list_arPages);
        
        //Corto el array obteniendo solo los items que corresponden a la página $iPage
        $arIdPaginate = $oPaginate->get_array_items();
        //Obtengo los valores autonumericos de la columna Id
        $arIdPaginate = $this->get_field_values($arIdPaginate, "Id");
        
        $sSQL = "SELECT DISTINCT a.Id, 
                a.Code, c.Name, 
                a.DateP AS FechaHydra, 
                SUBSTRING(a.DateP,7,2)+'/'+SUBSTRING(a.DateP,5,2)+'/'+SUBSTRING(a.DateP,1,4) AS Fecha,
                a.Code_Contact, co.Description AS Contacto,
                a.Code_User, u.Description AS Comercial
                FROM activities AS a
                LEFT JOIN clients AS c
                ON a.Code_Account = c.Code
                LEFT JOIN contacts AS co
                ON co.Code = a.Code_Contact
                LEFT JOIN core_users AS u
                ON a.Code_User = u.Code

                WHERE 1=1
                AND a.Id IN ".$this->build_sqlin($arIdPaginate,true);
        $sSQL .= " ORDER BY a.DateP DESC";
        //bug($arIdPaginate,$sSQL); die;
        $arData = $this->oDB->query($sSQL);
        //bug($arData,$sSQL); 
        return $arData;           
    }


    /**
     * Sobreescribo el metodo pq la generacion se hace segun el cliente
     * y no de forma aleatoria
     */
    public function get_key_by_fields() 
    {
        $sCodeContact = $this->_code_contact;
        $sCodeNewType = $this->_code_newtype;
        $sFecha = $this->_datep;
        
        $sNewKey = "$sCodeContact-$sCodeNewType-$sFecha";
        $this->_is_key_in_table = $this->is_value_in_field($sNewKey);
        if($this->_is_key_in_table)
            return "in use";
        return $sNewKey;
    }
    
    public function belongs_to_user()
    {
        $sCodeContact = $this->_code;
        $sCodeUser = $this->_code_user_session;
        $sSQL = "SELECT Code FROM contacts 
                 WHERE Code_Account IN 
                    (
                        SELECT Code 
                        FROM clients 
                        WHERE Propietario IN
                        (
                            SELECT Code FROM get_code_vendedores('$sCodeUser')
                        )
                     )
                 AND Code='$sCodeContact'";
        $arField = $this->oDB->query($sSQL);
        $sCode = $arField[0]["Code"];
        return ($sCode==$sCodeContact);
    }
    
    public function update(){}
    public function delete(){}
    public function insert(){}    
    
    public function get_code(){ return $this->_code; }
    public function get_code_account(){ return $this->_code_account; }
    public function get_code_contact(){ return $this->_code_contact; }
    public function get_code_newtype(){ return $this->_code_newtype; }
    public function get_code_newtype_next(){ return $this->_code_newtype_next; }
    public function get_code_result(){ return $this->_code_result; }
    //public function get_code_type_issue(){ return $this->_code_type_issue; }
    public function get_code_user(){ return $this->_code_user; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_date_next(){ return $this->_date_next; }
    public function get_datep(){ return $this->_datep; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    //public function get_description(){ return $this->_description; }
    public function get_end_time(){ return $this->_end_time; }
    public function get_id(){ return $this->_id; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_notas_centralita(){ return $this->_notas_centralita; }
    public function get_notes(){ return $this->_notes; }
    public function get_start_time(){ return $this->_start_time; }
            
    public function set_code($value){ $this->_code = $value; }
    public function set_code_account($value){ $this->_code_account = $value; $this->oClient = new ModelClients(null,$value); }
    public function set_code_contact($value){ $this->_code_contact = $value; $this->oContact = new ModelContacts(null,$value); }
    public function set_code_newtype($value){ $this->_code_newtype = $value; }
    public function set_code_newtype_next($value){ $this->_code_newtype_next = $value; }
    public function set_code_result($value){ $this->_code_result = $value; }
    //public function set_code_type_issue($value){ $this->_code_type_issue = $value; }
    public function set_code_user($value){ $this->_code_user = $value; $this->oUser = new ModelCoreUsers(null, $value);}
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_date_next($value){ $this->_date_next = $value; }
    public function set_datep($value){ $this->_datep = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    //public function set_description($value){ $this->_description = $value; }
    public function set_end_time($value){ $this->_end_time = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_notas_centralita($value){ $this->_notas_centralita = $value; }
    public function set_notes($value){ $this->_notes = $value; }
    public function set_start_time($value){ $this->_start_time = $value; }
    
    public function get_contact_description(){ return $this->_contact_description; }
    public function set_contact_description($value){ $this->_contact_description = $value; }   
    
    public function get_mtb_last_name2(){ return $this->_mtb_last_name2; }
    public function set_mtb_last_name2($value){ $this->_mtb_last_name2 = $value; }
    
    public function get_user_description(){ return $this->_user_description; }
    public function set_user_description($value){ $this->_user_description = $value; }       
    
    /**
     * La clave se genera manualmente contacto-accion-fecha,
     * si existe una visita con estos valores entonces se asigna a true
     * @return boolean 
     */
    public function is_key_in_table()
    {
        return $this->_is_key_in_table;
    }
    
}
