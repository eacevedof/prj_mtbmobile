<?php
//Model configuration
class ModelModuleTabButton extends MainModel
{
    private $_action;
    private $_active;
    private $_code_button;
    private $_code_module;
    private $_code_module_to;
    private $_code_tab;
    private $_code_tab_to;
    private $_code_type;
    private $_create_date;
    private $_create_user;
    private $_description;
    private $_function_to;
    private $_icon;
    private $_id;
    private $_modify_date;
    private $_modify_user;
    private $_name;
    private $_orden;
    private $_status_mobile;
    private $_theme;

    public function __construct($code_button, $code_module="",$code_tab="", $action="",$active="",
        $code_module_to="", $code_tab_to="", $code_type="", $create_date="",
        $create_user="", $description="", $function_to="", $icon="", $id="", $modify_date="",
        $modify_user="", $name="", $orden="", $status_mobile="", $theme="") 
    {
        global $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "m_module_tab_buttons";
        
        $this->_action = $action;
        $this->_active = $active;
        $this->_code_button = $code_button;
        $this->_code_module = $code_module;
        $this->_code_module_to = $code_module_to;
        $this->_code_tab = $code_tab;
        $this->_code_tab_to = $code_tab_to;
        $this->_code_type = $code_type;
        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_description = $description;
        $this->_function_to = $function_to;
        $this->_icon = $icon;
        $this->_id = $id;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_name = $name;
        $this->_orden = $orden;
        $this->_status_mobile = $status_mobile;
        $this->_theme = $theme;        
    }
    
    public function load_by_code()
    {
        //ERROR no se puede cargar el modelo pq devuelve más de un registro
        $sSQL = "SELECT * FROM $this->_table_name 
                WHERE Code_Button='$this->_code_button'
                ";
        $this->load_from_sql($sSQL);        
    }
    
    public function get_buttons_by_module_and_tab($sCodeModule="",$sCodeTab="")
    {
        if(empty($sCodeModule))
            $sCodeModule = $this->_code_module;
        
        if(empty($sCodeTab))
            $sCodeTab = $this->_code_tab;
        
        $sSQL = "SELECT Action, Active, Code_Button, Code_Module, Code_Module_To,
                 Code_Tab, Code_Tab_To, Code_Type, Description, Function_To,
                Icon, Id, Name, Orden, status_mobile,Theme 
                FROM $this->_table_name 
                WHERE Code_Module='$sCodeModule'
                AND Code_Tab='$sCodeTab'
                ";
        $arButtons = $this->oDB->query($sSQL);
        return $arButtons;
    }
    
    protected function set_all_properties($arData) 
    {
        $this->_action = $arData["Action"];
        $this->_active = $arData["Active"];
        $this->_code_button = $arData["Code_Button"];
        $this->_code_module = $arData["Code_Module"];
        $this->_code_module_to = $arData["Code_Module_To"];
        $this->_code_tab = $arData["Code_Tab"];
        $this->_code_tab_to = $arData["Code_Tab_To"];
        $this->_code_type = $arData["Code_Type"];
        $this->_create_date = $arData["Create_Date"];
        $this->_create_user = $arData["Create_User"];
        $this->_description = $arData["Description"];
        $this->_function_to = $arData["Function_To"];
        $this->_icon = $arData["Icon"];
        $this->_id = $arData["Id"];
        $this->_modify_date = $arData["Modify_Date"];
        $this->_modify_user = $arData["Modify_User"];
        $this->_name = $arData["Name"];
        $this->_orden = $arData["Orden"];
        $this->_status_mobile = $arData["status_mobile"];
        $this->_theme = $arData["Theme"];
    }
    
    public function get_action(){ return $this->_action; }
    public function get_active(){ return $this->_active; }
    public function get_code_button(){ return $this->_code_button; }
    public function get_code_module(){ return $this->_code_module; }
    public function get_code_module_to(){ return $this->_code_module_to; }
    public function get_code_tab(){ return $this->_code_tab; }
    public function get_code_tab_to(){ return $this->_code_tab_to; }
    public function get_code_type(){ return $this->_code_type; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_description(){ return $this->_description; }
    public function get_function_to(){ return $this->_function_to; }
    public function get_icon(){ return $this->_icon; }
    public function get_id(){ return $this->_id; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_name(){ return $this->_name; }
    public function get_orden(){ return $this->_orden; }
    public function get_status_mobile(){ return $this->_status_mobile; }
    public function get_theme(){ return $this->_theme; }

    public function set_action($value){ $this->_action = $value; }
    public function set_active($value){ $this->_active = $value; }
    public function set_code_button($value){ $this->_code_button = $value; }
    public function set_code_module($value){ $this->_code_module = $value; }
    public function set_code_module_to($value){ $this->_code_module_to = $value; }
    public function set_code_tab($value){ $this->_code_tab = $value; }
    public function set_code_tab_to($value){ $this->_code_tab_to = $value; }
    public function set_code_type($value){ $this->_code_type = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_function_to($value){ $this->_function_to = $value; }
    public function set_icon($value){ $this->_icon = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_name($value){ $this->_name = $value; }
    public function set_orden($value){ $this->_orden = $value; }
    public function set_status_mobile($value){ $this->_status_mobile = $value; }
    public function set_theme($value){ $this->_theme = $value; }
}