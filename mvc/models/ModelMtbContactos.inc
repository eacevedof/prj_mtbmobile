<?php
class ModelMtbContactos extends MainModel implements InterfaceModel
{
    protected $_code;
    
    private $_last_name2;
    private $_codigo_erp;
    private $_empresa;
    private $_enviado_a_erp;
    private $_modify_date;
    private $_modify_user;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    
    public $oContact;
    
    public function __construct(ModelContacts $oContact, $code="",$codigo_erp="", 
            $create_date="",$create_user="",$delete_date="",$delete_user="",
            $empresa="",$enviado_a_erp="", $last_name2="",$modify_date="",$modify_user="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "MTB_Contactos";
        $this->_table_code = "5140";
        
        $sSQL = "SELECT Code, Last_Name2, Empresa, Codigo_Erp, Enviado_a_Erp, 
                Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date
                FROM MTB_Contactos ";
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->oContact = new ModelContacts(null,$code);
        if(!empty($oContact))
        {
            $this->oContact = $oContact;
            $this->_code = $oContact->get_code();
        }
        
        $this->_last_name2 = $last_name2;
        $this->_codigo_erp = $codigo_erp;
        $this->_empresa = $empresa;
        $this->_enviado_a_erp = $enviado_a_erp;
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
        $this->oContact = new ModelContacts(null, $this->_code);
        $this->oContact->load_by_code();
    }
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arRow["Code"];
        $this->_last_name2 = $arRow["Last_Name2"];
        $this->_codigo_erp = $arRow["Codigo_Erp"];
        $this->_empresa = $arRow["Empresa"];
        $this->_enviado_a_erp = $arRow["Enviado_a_Erp"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
    }
    
    public function get_mobile_list(){}
    public function update(){}
    public function delete(){}
    public function insert(){}    
    
    public function get_code(){ return $this->_code; }
    public function get_last_name2(){ return $this->_last_name2; }
    public function get_codigo_erp(){ return $this->_codigo_erp; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_empresa(){ return $this->_empresa; }
    public function get_enviado_a_erp(){ return $this->_enviado_a_erp; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    
    public function set_code($value){ $this->_code = $value; }
    public function set_last_name2($value){ $this->_last_name2 = $value; }
    public function set_codigo_erp($value){ $this->_codigo_erp = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_empresa($value){ $this->_empresa = $value; }
    public function set_enviado_a_erp($value){ $this->_enviado_a_erp = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    
}
