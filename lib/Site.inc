<?php
class Site
{
    //Database
    //private $oDB;
    //Framework Object Models
    //public $oConfiguration = null;
    /**
     * @var ModelModule
     */
    //public $oModule= null;
    /**
     * @var ModelModuleTab
     */
    //public $oModuleTab = null;
    /**
     * @var ModelModuleTabButton
     */
    //public $oModuleTabButtons = null;
    /**
     * @var ModelModuleTabField
     */
    //public $oModuleTabFields = null;
    //private $oCUtilities = null;
    
    private $_module_requested_code = "";
    private $_module_requested_code_hydra = "";
    
    //Data from URI
    private $_module_name_requested = "";
    private $_tab_name_requested = "";
    
    private $_message = array();
    private $_action_name_requested = "";

    private $_controller_name = "";
    private $_controller_method_name = "";
    private $_controller_file_name = "";
    
    //Especie de enrutador
    private $_arControllerModule = array();
    
    private $_project_name = "";
    private $_page_title = "";
    private $_logged_user_name = "";
    private $_logged_user_code = "";
    
    private $_today_date = "";
    private $_isUsingAjax = false;
    
    # Constructor
    public function __construct()
    {
        //bug($_POST);die;
        Debug::set_vardump($_SESSION, "En sesion");
        //Debug::set_vardump($_GET,"GET en site");
        //Debug::set_vardump($_POST,"POST en site");
        //Carga array de rutas
        $this->set_controller_module();
        $this->even_post_and_get();
        Debug::set_vardump($_GET, "GET en site despues de even");
        Debug::set_vardump($_POST, "POST en site despues de even");        
        
        //http://localhost/proy_mtbmobile/www/index.php?module=clients&tab=DETAIL&Code=04157
        //$this->build_post_from_get();
        $this->_module_name_requested = $_POST["module"];
        Debug::set_vardump($this->_module_name_requested,"this->_module_name_requested");
        $this->_tab_name_requested = $_POST["tab"];
        $this->_action_name_requested = $_POST["action"];
        
        //Debe respetarse este orden. File_Name depende de controller_name
        $this->_controller_name = $this->get_controller_name_by_module();
        $this->_controller_file_name = $this->get_controller_file_name();
        $this->_controller_method_name = $this->get_method_by_tab();

        $arUser = $this->get_from_session("user_logged");
        $this->_logged_user_name = $arUser["Description"];
        $this->_logged_user_code = $arUser["Code"];
        $this->_logged_user_code_manager = $arUser["Code_Manager"];
        $this->_project_name = $this->_logged_user_name;
        $this->_page_title = "MtbKey Mobile - $this->_logged_user_name";
        $this->_today_date = date("d/m/Y");
        
        //Debug::set_message(var_export($this, true),"SITE en su constructor");
        //Las configuraciones 
        //$this->oConfiguration = new ModelConfiguration();
        
        //Carga las pestañas del módulo pedido
        //$this->oModuleTab = new ModelModuleTab($this->_module_name_requested,$this->_tab_name_requested);
        //$this->oModuleTab->load_by_codes();
 
        //$this->oModuleTabButtons = new ModelModuleTabButton(null,$this->_module_name_requested,$this->_tab_name_requested);
        /**
        //Asigna default en caso de no proporcionar un módulo con get
        $this->fix_to_default_module();
        //Asigna default en caso de no proporcionar un módulo con get
        $this->fix_to_default_tab();
         * 
         */
        //Carga los datos del módulo actual
        //$this->oModule = new ModelModule($this->_module_name_requested);
        //$this->oModule->load_by_code();
        //bug($this,"oSite");
    }
    
    private function set_controller_module()
    {
        $arModuleController = array();
        $arModuleController["clients"] = "ControllerClients";
        $arModuleController["contacts"] = "ControllerContacts";
        $arModuleController["activities"] = "ControllerActivities";
        $arModuleController["ajax"] = "ControllerAjax";
        $arModuleController["descuentos"] = "ControllerDiscounts";
        $arModuleController["propuestas"] = "ControllerProporsals";
        $arModuleController["ventas"] = "ControllerSales";
        
        $this->_arControllerModule = $arModuleController;
    }
    
    private function even_post_and_get()
    {
        if(!empty($_POST) && empty($_GET))
        {
            foreach($_POST as $key=>$Value)
            {
                $_GET[$key] = $Value;
            }
        }
        if(!empty($_GET)&& empty($_POST))
        {
            foreach($_GET as $key=>$Value)
            {
                $_POST[$key] = $Value;
            }            
        }
    }
    
    
    
    private function get_controller_name_by_module()
    {
        //Debug::set_vardump($_GET, "GET en controller by name");
        //Debug::set_vardump($_POST, "POST en controller by name");
        $sModuleName = $this->_module_name_requested;
        //bug($sModuleName,"módulo pedido");
        if(empty($sModuleName))
        {
            $sModuleName = "clients"; 
            Debug::set_message("No se ha especificado un módulo.");
        }
            
        $sControllerName = $this->_arControllerModule[$sModuleName];
        if(empty($sControllerName))
        {
            $sMessage="El módulo $sModuleName no existe se cargará clients";
            Debug::set_message($sMessage);
            $sControllerName = "ControllerClients";
        }

        return $sControllerName;
    }
    
    private function get_method_by_tab()
    {
        $sMethodName = $this->_tab_name_requested ;
        if($sMethodName=="list" || empty($sMethodName))
            $sMethodName = "basic_list";
        //bug($sMethodName);
        return $sMethodName;
    }
    
    public function get_controller_file_name()
    {
        $sControllerName = $this->_controller_name;
        return $sControllerName.".inc";
    }
    
    public function delete_from_session($key)
    {
        if(isset($_SESSION[$key]))
            unset($_SESSION[$key]);
    }
    
    private function is_user_logged()
    {
        if(!empty($_SESSION["user_logged"]))
        {
            return true;
        }
        return false;
    }
    
    public function set_in_session($sKey,$mxValue)
    {
        $_SESSION[$sKey] = $mxValue;
    }

    public function get_from_session($sKey="")
    {
        //bug($_SESSION[$sKey]);
        if(empty($sKey))
        {    
            return $_SESSION;
        }
        elseif(!empty($_SESSION[$sKey]))
        {
            return $_SESSION[$sKey];
        }
        return null;
    }
    
    public function get_module_name_requested()
    {
        return $this->_module_name_requested;
    }
    
    public function get_module_requested_code()
    {
        return $this->_module_requested_code;
    }
    
    public function get_module_requested_code_hydra()
    {
        return $this->_module_requested_code_hydra;
    }
    
    public function get_controller_name()
    {
        return $this->_controller_name;
    }
    
    public function get_controller_method_name()
    {
        return $this->_controller_method_name;
    }
    /*
    public function get_model_configuration()
    {
        return $this->oConfiguration;
    }*/
    public function get_today_date()
    {
        return $this->_today_date;
    }
    
    public function get_logged_user_code()
    {
        return $this->_logged_user_code;
    }
   
    public function get_logged_user_code_manager()
    {
        return $this->_logged_user_code_manager;
    }
       
    public function get_logged_user_name()
    {
        return $this->_logged_user_name;
    }
    
    public function set_logged_user_name($sName)
    {
        $this->_logged_user_name = $sName;
    }
    
    public function get_project_name()
    {
        return $this->_project_name;
    }
    
    public function get_page_title()
    {
        return $this->_page_title;
    }
    
    public function set_page_title($sTitle)
    {
        $this->_page_title = $sTitle;
    }
    
    public function get_messages()
    {
        $arMessages = $this->_message;
        $sReturn = "";
        foreach ($arMessages as $sMessage)
        {
            $sReturn .= "$sMessage \n";
        }
        return $sReturn;
    }

    public function use_ajax($useAjax=true)
    {
        $this->_isUsingAjax = $useAjax;
    }
    
    public function is_using_ajax()
    {
        return $this->_isUsingAjax;
    }
}
