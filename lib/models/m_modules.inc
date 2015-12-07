<?php
//Model module
class ModelModule extends MainModel
{
    //keys
    private $_code_module;
    
    private $_active;
    private $_code_hydra;
    private $_controller_name;
    private $_create_date;
    private $_create_user;
    private $_delete_permission;
    private $_desc_field;
    private $_description;
    private $_folder;
    private $_icon;
    private $_icon_file;
    private $_id;
    private $_insert_permission;
    private $_key_field;
    private $_list_field;
    private $_main_table;
    private $_modify_date;
    private $_modify_user;
    private $_name;
    private $_rel_field;
    private $_select_permission;
    private $_show_order;
    private $_status_mobile;
    private $_update_permission;

    private $_splited_key_fields;
    private $_splited_rel_fields;
    private $_splitted_desc_fields;
    private $_splitted_list_fields;
    
    public function __construct
    (
        $code_module="", $active="",
        $code_hydra="", $controller_name="", $create_date="", $create_user="",
        $delete_permission="", $desc_field="", $description="", $folder="", $icon="", $icon_file="",
        $id="", $insert_permission="", $key_field="", $list_field="", $main_table="", $modify_date="",
        $modify_user="", $name="", $rel_field="", $select_permission="", $show_order="", $status_mobile="",
        $update_permission=""
    )
    {
            global $oDB;
            $this->oDB = $oDB;
            $this->_table_name = "m_modules";

            $this->_active = $active;
            $this->_code_hydra = $code_hydra;
            $this->_code_module = $code_module;
            $this->_controller_name = $controller_name;
            $this->_create_date = $create_date;
            $this->_create_user = $create_user;
            $this->_delete_permission = $delete_permission;
            $this->_desc_field = $desc_field; //explode
            $this->_description = $description;
            $this->_folder = $folder;
            $this->_icon = $icon;
            $this->_icon_file = $icon_file;
            $this->_id = $id;
            $this->_insert_permission = $insert_permission;
            $this->_key_field = $key_field; //explode
            $this->_list_field = $list_field; //explode
            $this->_main_table = $main_table;
            $this->_modify_date = $modify_date;
            $this->_modify_user = $modify_user;
            $this->_name = $name;
            $this->_rel_field = $rel_field; //explode
            $this->_select_permission = $select_permission;
            $this->_show_order = $show_order;
            $this->_status_mobile = $status_mobile;
            $this->_update_permission = $update_permission;
    }//end construct
    
    /**
     * Antes de llamar este metodo se ha de asignar un valor a code_module
     */
    public function load_by_code()
    {
        $sSQL = "SELECT * FROM $this->_table_name WHERE Code_Module='$this->_code_module'";
        $this->load_from_sql($sSQL);
    }
    
    /**
     * Devuelve un array con un menu y sus módulos
     * @param 
     */
    public function get_menu($code_module="")
    {
        $sSQL = "SELECT Code_Module, Icon, Icon_File, Active, Description FROM $this->_table_name
                 WHERE 1=1
                ";
        if(!empty($code_module))
            $sSQL .= " AND Code_Module = '$code_module' ";
        
        $sSQL .= "ORDER BY Description ASC";
        $arData = $this->oDB->query($sSQL);
        return $arData;
    }
    
    public function getMenu()
    {
         
        foreach($this->oSite->oMModule->get_table() as $code_module => $module_prop )
        {
            $return["modules"][$code_module]["Icon"] = $module_prop["Icon"];
            $return["modules"][$code_module]["Icon_File"] = $module_prop["Icon_File"];
            $return["modules"][$code_module]["Act"] = $module_prop["Act"];
            //bug($module_prop,"get_menu",1);
            if($this->Translate->{$this->oSite->module}->{$module_prop["Description"]})
                    $return["modules"][$code_module]["Description"] = $this->Translate->{$this->oSite->module}->{$module_prop["Description"]};
            else
                    $return["modules"][$code_module]["Description"] = $module_prop["Description"];
        }
        return $return;
    }    
    
    protected function set_all_properties($arData)
    {
        $this->_active = $arData["Active"];
        $this->_code_hydra = $arData["Code_Hydra"];
        $this->_code_module = $arData["Code_Module"];
        $this->_controller_name = $arData["Controller_Name"];
        $this->_create_date = $arData["Create_Date"];
        $this->_create_user = $arData["Create_User"];
        $this->_delete_permission = $arData["Delete_Permission"];
        $this->_desc_field = $arData["Desc_Field"];
        $this->_description = $arData["Description"];
        $this->_folder = $arData["Folder"];
        $this->_icon = $arData["Icon"];
        $this->_icon_file = $arData["Icon_File"];
        $this->_id = $arData["Id"];
        $this->_insert_permission = $arData["Insert_Permission"];
        $this->_key_field = $arData["Key_Field"];
        $this->_list_field = $arData["List_Field"];
        $this->_main_table = $arData["Main_Table"];
        $this->_modify_date = $arData["Modify_Date"];
        $this->_modify_user = $arData["Modify_User"];
        $this->_name = $arData["Name"];
        $this->_rel_field = $arData["Rel_Field"];
        $this->_select_permission = $arData["Select_Permission"];
        $this->_show_order = $arData["Show_Order"];
        $this->_status_mobile = $arData["status_mobile"];
        $this->_update_permission = $arData["Update_Permission"];
        
        $this->_splitted_desc_fields = $this->get_fieldvalue_exploded($this->_desc_field);
        $this->_splited_key_fields = $this->get_fieldvalue_exploded($this->_key_field);
        $this->_splitted_list_fields = $this->get_fieldvalue_exploded($this->_list_field);
        $this->_splited_rel_fields = $this->get_fieldvalue_exploded($this->_rel_field);
    }
         
    public function get_controller_name_by_module($code_module="")
    {
        if(empty($code_module))
            $code_module = $this->_code_module;
        
        $sSQL= "SELECT Controller_Name FROM $this->_table_name WHERE Code_Module='$code_module'";
        $arData = $this->oDB->query($sSQL);
        return $arData[0]["Controller_Name"];
    }
    
    //Requested action select, insert, delete, update
    public function has_permission($sRequestedModule="", $sRequestedAction="")
    {
        $sSQL = "";
        $arRows = $this->oDB->query($sSQL);
        if($arRows[0][0]==0)
        {
            return false;
        }
        elseif($arRows[0][0]==1)
        {
            return true;
        }
            
        return -1;
    }
        
    //===========================     GETS AND SETS     ===========================    
    public function get_active(){ return $this->_active; }
    public function get_code_hydra(){ return $this->_code_hydra; }
    public function get_code_module(){ return $this->_code_module; }
    public function get_controller_name(){ return $this->_controller_name; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_permission(){ return $this->_delete_permission; }
    public function get_desc_field(){ return $this->_desc_field; }
    public function get_description(){ return $this->_description; }
    public function get_folder(){ return $this->_folder; }
    public function get_icon(){ return $this->_icon; }
    public function get_icon_file(){ return $this->_icon_file; }
    public function get_id(){ return $this->_id; }
    public function get_insert_permission(){ return $this->_insert_permission; }
    public function get_key_field(){ return $this->_key_field; }
    public function get_list_field(){ return $this->_list_field; }
    public function get_main_table(){ return $this->_main_table; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_name(){ return $this->_name; }
    public function get_rel_field(){ return $this->_rel_field; }
    public function get_select_permission(){ return $this->_select_permission; }
    public function get_show_order(){ return $this->_show_order; }
    public function get_status_mobile(){ return $this->_status_mobile; }
    public function get_update_permission(){ return $this->_update_permission; }  
    
    //
    public function set_active($value){ $this->_active = $value; }
    public function set_code_hydra($value){ $this->_code_hydra = $value; }
    public function set_code_module($value){ $this->_code_module = $value; }
    public function set_controller_name($value){ $this->_controller_name = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_permission($value){ $this->_delete_permission = $value; }
    public function set_desc_field($value){ $this->_desc_field = $value; }
    public function set_description($value){ $this->_description = $value; }
    public function set_folder($value){ $this->_folder = $value; }
    public function set_icon($value){ $this->_icon = $value; }
    public function set_icon_file($value){ $this->_icon_file = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_insert_permission($value){ $this->_insert_permission = $value; }
    public function set_key_field($value){ $this->_key_field = $value; }
    public function set_list_field($value){ $this->_list_field = $value; }
    public function set_main_table($value){ $this->_main_table = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_name($value){ $this->_name = $value; }
    public function set_rel_field($value){ $this->_rel_field = $value; }
    public function set_select_permission($value){ $this->_select_permission = $value; }
    public function set_show_order($value){ $this->_show_order = $value; }
    public function set_status_mobile($value){ $this->_status_mobile = $value; }
    public function set_update_permission($value){ $this->_update_permission = $value; }    
}