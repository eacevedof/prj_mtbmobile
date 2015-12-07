<?php
//Model configuration
class ModelModuleTab extends MainModel
{
    //pk
    private $_code_module;
    private $_code_tab;
    private $_code_type;
    
    private $_active;
    private $_create_date;
    private $_create_user;
    private $_description;
    private $_element_desclist;
    private $_element_keyfield;
    private $_element_listfield;
    private $_element_table;
    private $_icon;
    private $_id;
    private $_list_descriptions;
    private $_list_dividers;
    private $_list_header;
    private $_modify_date;
    private $_modify_user;
    private $_name;
    private $_orden;
    private $_readonly;
    private $_relational_keyfield;
    private $_relational_module;
    private $_relational_table;
    private $_status_mobile;
    
    private $_splitted_element_desclist;
    private $_splitted_element_keyfield;
    private $_splitted_element_listfield;
    
    public function __construct($code_module="", $code_tab="", $code_type="", $active="", 
        $create_date="", $create_user="", $description="", $element_desclist="",
        $element_keyfield="", $element_listfield="", $element_table="", $icon="",
        $id="", $list_descriptions="", $list_dividers="", $list_header="", $modify_date="",
        $modify_user="", $name="", $orden="", $readonly="", $relational_keyfield="",
        $relational_module="", $relational_table="", $status_mobile="") 
    {
        global $oDB;
        $this->oDB = $oDB;
        $this->_message = "";
        $this->_table_name = "m_module_tabs";
        
        $this->_code_module = $code_module;
        $this->_code_tab = $code_tab;
        $this->_code_type = $code_type;
        $this->_active = $active;
        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_description = $description;
        $this->_element_desclist = $element_desclist; //explode
        $this->_element_keyfield = $element_keyfield; //explode
        $this->_element_listfield = $element_listfield; //explode
        $this->_element_table = $element_table;
        $this->_icon = $icon;
        $this->_id = $id;
        $this->_list_descriptions = $list_descriptions;
        $this->_list_dividers = $list_dividers;
        $this->_list_header = $list_header;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_name = $name;
        $this->_orden = $orden;
        $this->_readonly = $readonly;
        $this->_relational_keyfield = $relational_keyfield;
        $this->_relational_module = $relational_module;
        $this->_relational_table = $relational_table;
        $this->_status_mobile = $status_mobile;        
    }
    
    /**
     * Antes de llamar este metodo se ha de asignar un valor a code_module
     */
    public function load_by_codes()
    {
        $sSQL = "SELECT * FROM $this->_table_name 
                WHERE Code_Module='$this->_code_module'
                AND Code_Tab='$this->_code_tab'
                ";
        $this->load_from_sql($sSQL);
    }
    
    protected function set_all_properties($arData)
    {
        $this->_active = $arData["Active"];
        $this->_code_module = $arData["Code_Module"];
        $this->_code_tab = $arData["Code_Tab"];
        $this->_code_type = $arData["Code_Type"];
        $this->_create_date = $arData["Create_Date"];
        $this->_create_user = $arData["Create_User"];
        $this->_description = $arData["Description"];
        $this->_element_desclist = $arData["Element_DescList"];
        $this->_element_keyfield = $arData["Element_Keyfield"];
        $this->_element_listfield = $arData["Element_ListField"];
        $this->_element_table = $arData["Element_Table"];
        $this->_icon = $arData["Icon"];
        $this->_id = $arData["Id"];
        $this->_list_descriptions = $arData["List_Descriptions"];
        $this->_list_dividers = $arData["List_Dividers"];
        $this->_list_header = $arData["List_Header"];
        $this->_modify_date = $arData["Modify_Date"];
        $this->_modify_user = $arData["Modify_User"];
        $this->_name = $arData["Name"];
        $this->_orden = $arData["Orden"];
        $this->_readonly = $arData["ReadOnly"];
        $this->_relational_keyfield = $arData["Relational_Keyfield"];
        $this->_relational_module = $arData["Relational_Module"];
        $this->_relational_table = $arData["Relational_Table"];
        $this->_status_mobile = $arData["status_mobile"];     
        
        $this->_splitted_element_desclist = $this->get_fieldvalue_exploded($this->_element_desclist);
        $this->_splitted_element_keyfield = $this->get_fieldvalue_exploded($this->_element_keyfield);
        $this->_splitted_element_listfield = $this->get_fieldvalue_exploded($this->_element_listfield);
    }

    public function get_active(){ return $this->_active; }
    public function get_code_module(){ return $this->_code_module; }
    public function get_code_tab(){ return $this->_code_tab; }
    public function get_code_type(){ return $this->_code_type; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_description(){ return $this->_description; }
    public function get_element_desclist(){ return $this->_element_desclist; }
    public function get_element_keyfield(){ return $this->_element_keyfield; }
    public function get_element_listfield(){ return $this->_element_listfield; }
    public function get_element_table(){ return $this->_element_table; }
    public function get_icon(){ return $this->_icon; }
    public function get_id(){ return $this->_id; }
    public function get_list_descriptions(){ return $this->_list_descriptions; }
    public function get_list_dividers(){ return $this->_list_dividers; }
    public function get_list_header(){ return $this->_list_header; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_name(){ return $this->_name; }
    public function get_orden(){ return $this->_orden; }
    public function get_readonly(){ return $this->_readonly; }
    public function get_relational_keyfield(){ return $this->_relational_keyfield; }
    public function get_relational_module(){ return $this->_relational_module; }
    public function get_relational_table(){ return $this->_relational_table; }
    public function get_status_mobile(){ return $this->_status_mobile; }
    
    public function set_active($value){ $this->_active = $value; }
    public function set_code_module($value){ $this->_code_module = $value; }
    public function set_code_tab($value){ $this->_code_tab = $value; }
    public function set_code_type($value){ $this->_code_type = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_element_desclist($value){ $this->_element_desclist = $value; }
    public function set_element_keyfield($value){ $this->_element_keyfield = $value; }
    public function set_element_listfield($value){ $this->_element_listfield = $value; }
    public function set_element_table($value){ $this->_element_table = $value; }
    public function set_icon($value){ $this->_icon = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_list_descriptions($value){ $this->_list_descriptions = $value; }
    public function set_list_dividers($value){ $this->_list_dividers = $value; }
    public function set_list_header($value){ $this->_list_header = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_name($value){ $this->_name = $value; }
    public function set_orden($value){ $this->_orden = $value; }
    public function set_readonly($value){ $this->_readonly = $value; }
    public function set_relational_keyfield($value){ $this->_relational_keyfield = $value; }
    public function set_relational_module($value){ $this->_relational_module = $value; }
    public function set_relational_table($value){ $this->_relational_table = $value; }
    public function set_status_mobile($value){ $this->_status_mobile = $value; }    
}