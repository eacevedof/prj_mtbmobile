<?php
//Model configuration
class ModelModuleTabField extends MainModel
{
    private $_code_module;
    private $_code_tab;
    private $_code_type;
    
    private $_autofield;
    private $_create_date;
    private $_create_user;
    private $_description;
    private $_editable;
    private $_id;
    private $_keyfield;
    private $_modify_date;
    private $_modify_user;
    private $_name;
    private $_ord;
    private $_required;
    private $_source_code;
    private $_source_desc;
    private $_source_table;
    private $_status_mobile;
    
    public function __construct($autofield="", $code_module="", $code_tab="",
            $code_type="", $create_date="", $create_user="", $description="",
            $editable="", $id="", $keyfield="", $modify_date="", $modify_user="",
            $name="", $ord="", $required="", $source_code="", $source_desc="",
            $source_table="", $status_mobile="") 
    {
        global $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "m_module_tab_fields";

        $this->_code_module = $code_module;
        $this->_code_tab = $code_tab;
        $this->_code_type = $code_type;
        
        $this->_autofield = $autofield;
        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_description = $description;
        $this->_editable = $editable;
        $this->_id = $id;
        $this->_keyfield = $keyfield;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_name = $name;
        $this->_ord = $ord;
        $this->_required = $required;
        $this->_source_code = $source_code;
        $this->_source_desc = $source_desc;
        $this->_source_table = $source_table;
        $this->_status_mobile = $status_mobile;        
        
    }

    protected function set_all_properties($arData)
    {
        $this->_autofield = $arData["Autofield"];
        $this->_code_module = $arData["Code_Module"];
        $this->_code_tab = $arData["Code_Tab"];
        $this->_code_type = $arData["Code_Type"];
        $this->_create_date = $arData["Create_Date"];
        $this->_create_user = $arData["Create_User"];
        $this->_description = $arData["Description"];
        $this->_editable = $arData["Editable"];
        $this->_id = $arData["Id"];
        $this->_keyfield = $arData["Keyfield"];
        $this->_modify_date = $arData["Modify_Date"];
        $this->_modify_user = $arData["Modify_User"];
        $this->_name = $arData["Name"];
        $this->_ord = $arData["Ord"];
        $this->_required = $arData["Required"];
        $this->_source_code = $arData["Source_Code"];
        $this->_source_desc = $arData["Source_Desc"];
        $this->_source_table = $arData["Source_Table"];
        $this->_status_mobile = $arData["status_mobile"];        
    }

    public function get_autofield(){ return $this->_autofield; }
    public function get_code_module(){ return $this->_code_module; }
    public function get_code_tab(){ return $this->_code_tab; }
    public function get_code_type(){ return $this->_code_type; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_description(){ return $this->_description; }
    public function get_editable(){ return $this->_editable; }
    public function get_id(){ return $this->_id; }
    public function get_keyfield(){ return $this->_keyfield; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_name(){ return $this->_name; }
    public function get_ord(){ return $this->_ord; }
    public function get_required(){ return $this->_required; }
    public function get_source_code(){ return $this->_source_code; }
    public function get_source_desc(){ return $this->_source_desc; }
    public function get_source_table(){ return $this->_source_table; }
    public function get_status_mobile(){ return $this->_status_mobile; }

    public function set_autofield($value){ $this->_autofield = $value; }
    public function set_code_module($value){ $this->_code_module = $value; }
    public function set_code_tab($value){ $this->_code_tab = $value; }
    public function set_code_type($value){ $this->_code_type = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_editable($value){ $this->_editable = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_keyfield($value){ $this->_keyfield = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_name($value){ $this->_name = $value; }
    public function set_ord($value){ $this->_ord = $value; }
    public function set_required($value){ $this->_required = $value; }
    public function set_source_code($value){ $this->_source_code = $value; }
    public function set_source_desc($value){ $this->_source_desc = $value; }
    public function set_source_table($value){ $this->_source_table = $value; }
    public function set_status_mobile($value){ $this->_status_mobile = $value; }
}