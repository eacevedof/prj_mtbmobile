<?php
class ModelContacts extends MainModel implements InterfaceModel
{
    protected $_code;
    
    private $_cargo;
    private $_code_account;

    private $_description;
    private $_email;
    private $_fax;
    private $_first_name;
    private $_last_name;
    private $_tlf1;
    private $_tlf2;
    private $_modify_date;
    private $_modify_user;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    
    private $_full_name; //Para busquedas
    private $_mtb_last_name2;
    
    public $oClient;
    public $oMtbCliente;
    
    public function __construct($code_user_session="", $code="", ModelClients $oClient=null, $code_account="",  
             $description="", $tlf1="", $first_name="", $last_name="", $tlf2="",
            $cargo="", $fax="", $email="", $create_date="",$create_user="",$delete_date="",
            $delete_user="",$modify_date="",$modify_user="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "contacts";
        $this->_table_code = "334"; 
        $this->_code_user_session = $code_user_session;
        
        $sSQL = "SELECT  Code, Description, Code_Account, Tlf1, First_Name, Last_Name, Tlf2, Cargo, Fax, Email
                 FROM contacts ";
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->_code_account = $code_account;
        $this->oClient = new ModelClients($code_user_session,$code_account);
        $this->oMtbCliente = new ModelMtbClientes($this->oClient);
        if(!empty($oClient))
        {
            $this->oClient = $oClient; 
            $this->_code_account = $oClient->get_code();
            $this->oMtbCliente = new ModelMtbClientes($oClient);
        }
        //bug($this->_code_account); die;
        
        $this->_cargo = $cargo;
        $this->_description = $description;
        $this->_email = $email;
        $this->_fax = $fax;
        $this->_first_name = $first_name;
        $this->_last_name = $last_name;
        $this->_tlf1 = $tlf1;
        $this->_tlf2 = $tlf2;
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
        //bug($sSQL,"en load by code");
        $this->load_from_sql($sSQL);
        $this->oClient = new ModelClients(null, $this->_code_account);
        $this->oClient->load_by_code();
        $this->oMtbCliente = new ModelMtbClientes($this->oClient);
        $this->oMtbCliente->load_by_code();
    }
    
    protected function set_all_properties($arRow) 
    {
        $this->_cargo = $arRow["Cargo"];
        $this->_code = $arRow["Code"];
        $this->_code_account = $arRow["Code_Account"];
        $this->_description = $arRow["Description"];
        $this->_email = $arRow["Email"];
        $this->_fax = $arRow["Fax"];
        $this->_tlf1 = $arRow["Tlf1"];
        $this->_tlf2 = $arRow["Tlf2"];
        $this->_first_name = $arRow["First_Name"];
        $this->_last_name = $arRow["Last_Name"];
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
        $arValues["c.First_Name"] = $this->_first_name;
        $arValues["c.Last_Name"] = $this->_last_name;
        $arValues["mtb.Last_Name2"] = $this->_mtb_last_name2;
        
        $arValues["cargo.Description"] = $this->_cargo;
        $arMultiOr = array("c.First_Name","c.Last_Name","mtb.Last_Name2");
        $sCondSearch = $this->build_sql_like_for_search($arValues, $arMultiOr);
        
        $sSQL = "SELECT c.Id
                FROM contacts AS c
                LEFT JOIN MTB_Contactos AS mtb
                ON c.Code=mtb.Code
                LEFT JOIN contacts_array AS cargo
                ON c.Cargo = cargo.Code AND cargo.Type='3' --3:Cargo

                WHERE 1=1
                AND c.Code_Account='$sCodeAccount'
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
        
        $sSQL = "SELECT c.Code, 
                RTRIM(LTRIM(c.First_Name+' '+c.Last_Name+' '+mtb.Last_Name2)) AS Full_Name, 
                c.Tlf1 AS Telefono,
                cargo.Description AS Cargo

                FROM contacts AS c
                LEFT JOIN MTB_Contactos AS mtb
                ON c.Code=mtb.Code
                LEFT JOIN contacts_array AS cargo
                ON c.Cargo = cargo.Code AND cargo.Type='3' --3:Cargo

                WHERE 1=1
                AND c.Id IN ".$this->build_sqlin($arIdPaginate,true);
        //bug($arIdPaginate,$sSQL); die;
        $arData = $this->oDB->query($sSQL);
        //bug($arData,$sSQL); 
        return $arData;                
    }
    
    public function get_cargo_descripcion()
    {
        $sDescripcion = "";
        $sCodeCargo = $this->_cargo;
        $sSQL = "SELECT Description
                 FROM contacts_array AS cargo
                 WHERE cargo.Type='3' 
                 AND cargo.Code='$sCodeCargo'";
        $arRows = $this->oDB->query($sSQL);
        if(!empty($arRows))
            $sDescripcion = $arRows[0]["Description"];
        return $sDescripcion;
    }


    /**
     * Sobre escribo el metodo pq la generacion se hace segun el cliente
     * y no de forma aleatoria
     */
    public function get_key_erp_by_account() 
    {
        $codCliente = $this->_code_account;
        $sSQL = "SELECT MAX(CONVERT(int,Codigo_Erp)) AS maxcode
             FROM MTB_Contactos 
             INNER JOIN contacts 
             ON MTB_Contactos.Code = contacts.Code
             WHERE contacts.Code_Account='$codCliente'";        
        
        $arField = $this->oDB->query($sSQL);
        $iMaxCodigoErp = (int)$arField[0]["maxcode"];
        $iMaxCodigoErp++;
        return $iMaxCodigoErp;
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
    
    public function get_cargo(){ return $this->_cargo; }
    public function get_code(){ return $this->_code; }
    public function get_code_account(){ return $this->_code_account; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_description(){ return $this->_description; }
    public function get_email(){ return $this->_email; }
    public function get_fax(){ return $this->_fax; }
    public function get_first_name(){ return $this->_first_name; }
    public function get_last_name(){ return $this->_last_name; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_tlf1(){ return $this->_tlf1; }
    public function get_tlf2(){ return $this->_tlf2; }
            
    public function set_cargo($value){ $this->_cargo = $value; }
    public function set_code($value){ $this->_code = $value; }
    public function set_code_account($value){ $this->_code_account = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_email($value){ $this->_email = $value; }
    public function set_fax($value){ $this->_fax = $value; }
    public function set_first_name($value){ $this->_first_name = $value; }
    public function set_last_name($value){ $this->_last_name = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }    
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_tlf1($value){ $this->_tlf1 = $value; }
    public function set_tlf2($value){ $this->_tlf2 = $value; }    
    
    public function get_full_name(){ return $this->_full_name; }
    public function set_full_name($value){ $this->_full_name = $value; }    
    public function get_mtb_last_name2(){ return $this->_mtb_last_name2; }
    public function set_mtb_last_name2($value){ $this->_mtb_last_name2 = $value; }
}
