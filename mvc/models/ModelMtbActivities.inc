<?php
class ModelMtbActivities extends MainModel implements InterfaceModel
{
    protected $_code;

    //private $_code_oferta;
    private $_enviado_a_erp;
    private $_hora_proxima;
    private $_id;
    private $_resultado;

    private $_modify_date;
    private $_modify_user;
    private $_create_date;
    private $_create_user;
    private $_delete_date;
    private $_delete_user;
    
    public $oActivity;
    
    public function __construct(ModelActivities $oActivity, $code="",
            $enviado_a_erp="", $hora_proxima="", $id="", $resultado="",
            $create_date="",$create_user="",$delete_date="",$delete_user="")
    {
        global  $oDB;
        $this->oDB = $oDB;
        $this->_table_name = "MTB_Activities";
        $this->_table_code = "";
        
        $sSQL = "SELECT Id, Code, Enviado_a_Erp, Resultado, Hora_Proxima,
                 Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date
                 -- Code_Oferta
                 FROM MTB_Activities ";
        
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->oActivity = new ModelActivities(null, $code);
        if(!empty($oActivity))
        {
            $this->oActivity = $oActivity;
            $this->_code = $oActivity->get_code();
        }

        //$this->_code_oferta = $code_oferta;
        $this->_enviado_a_erp = $enviado_a_erp;
        $this->_hora_proxima = $hora_proxima;
        $this->_id = $id;
        $this->_resultado = $resultado;

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
        
        //$this->oActivity = new ModelActivities(null,$this->_code);
        //$this->oActivity->load_by_code();
        //bug($this->oActivity->oContact); die;
    }
    
    protected function set_all_properties($arRow) 
    {
        $this->_code = $arRow["Code"];
        //$this->_code_oferta = $arRow["Code_Oferta"];
        $this->_create_date = $arRow["Create_Date"];
        $this->_create_user = $arRow["Create_User"];
        $this->_delete_date = $arRow["Delete_Date"];
        $this->_delete_user = $arRow["Delete_User"];
        $this->_enviado_a_erp = $arRow["Enviado_a_Erp"];
        $this->_hora_proxima = $arRow["Hora_Proxima"];
        $this->_id = $arRow["Id"];
        $this->_modify_date = $arRow["Modify_Date"];
        $this->_modify_user = $arRow["Modify_User"];
        $this->_resultado = $arRow["Resultado"];
    }
    
    public function get_mobile_list(){}
    public function update(){}
    public function delete(){}
    public function insert(){}    
    
    public function get_code(){ return $this->_code; }
    //public function get_code_oferta(){ return $this->_code_oferta; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_enviado_a_erp(){ return $this->_enviado_a_erp; }
    public function get_hora_proxima(){ return $this->_hora_proxima; }
    public function get_id(){ return $this->_id; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_resultado(){ return $this->_resultado; }
    
    public function set_code($value){ $this->_code = $value; }
    //public function set_code_oferta($value){ $this->_code_oferta = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_enviado_a_erp($value){ $this->_enviado_a_erp = $value; }
    public function set_hora_proxima($value){ $this->_hora_proxima = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_resultado($value){ $this->_resultado = $value; }    
    
}
