<?php
class MainController 
{
    public $oModel = null;
    /**
     * @var Site
     */
    protected $oSite;   
    /**
     * @var Authorization
     */
    protected $oAuthorization;
    /**
     * @var GlobalParams
     */
    protected $oGParams;
    protected $oView;
    
    protected $_arSessionUser;
    protected $_list_page = 0;
    protected $_list_num_pages = 0;
    protected $_list_total_regs = 0;
    
    protected $_session_user_code;
    protected $_arFrgLinks;
    
    protected $_link_url_list = "";
    protected $_link_url_detail = ""; //habria que añadir los parametros
    
    protected $_is_error = false;
    protected $_error_message = "";
    protected $_succes_message = "";
    protected $_arErrorMessage = array();
    
    public function __construct()
    {
        /**
         * @var Site
         */
        global $oSite, $oGParams, $oAuthorization; 
        //bug(get_included_files());
        $this->oAuthorization = $oAuthorization;
        if(!$oAuthorization->is_user_logged())
            $oAuthorization->go_to_login();
        
        $this->oGParams = $oGParams;
        $this->oSite = $oSite;        
        $this->oView = new ViewDefault();
        
        $this->_arSessionUser = $this->oSite->get_from_session("user_logged");
        $this->_session_user_code = $this->_arSessionUser["Code"];
        $this->oSite->set_logged_user_name($this->_arSessionUser["Description"]);
        
    }
    
    /**
     * Comprueba que los campos pasados en fieldnames tengan un valor
     * @param array $arFieldNames Nombre de los campos de busqueda
     * @param string $sObjSession Nombre del objeto a guardar en sesion
     * @return boolean
     */
    protected function is_searching($arFieldNames=array(),$sObjSession="")
    {
        foreach($arFieldNames as $sFieldName)
        {
            $sFieldSch = "sch_$sFieldName";
            if($_POST[$sFieldSch]!="") return true;
        }
        $this->oSite->set_in_session($sObjSession, null);
        return false;
    }
    
    protected function load_foreign_links()
    {
        $arMenu = array();
        /**/
        $arMenu[] = array("module"=>"activities","description"=>"ACTIV.");
        $arMenu[] = array("module"=>"contacts","description"=>"CONTACT.");
        $arMenu[] = array("module"=>"activities","description"=>"HIST.","params"=>array("Code_Result"=>"1"));
        $arMenu[] = array("module"=>"descuentos","description"=>"CONDIC.");
        $arMenu[] = array("module"=>"propuestas","description"=>"PROPS.");
        $arMenu[] = array("module"=>"ventas","description"=>"VENTAS");
       
        $this->_arFrgLinks = $arMenu;
    }
    
    protected function is_saving()
    {
        return $_POST["action"]=="save";
    }
    
    protected function is_deleting()
    {
        return $_POST["action"]=="delete";
    }

    //Se usa para extraer los datos de los campos
    private function is_detail_field($sKey,$sPref)
    {
        $iRes = strpos($sKey, $sPref);
        //bug("$iRes - $sKey - $sPref");
        if($iRes===0)
            return true;
        return false;
    }
    
    protected function convert_to_hydra_date($sDate)
    {
        if(!empty($sDate))
        {
            $arDate = explode("/",$sDate);
            return "$arDate[2]$arDate[1]$arDate[0]";
        }
        return "";
    }
    /**
     * @param type $arPost $_POST array de post 
     * @param type $sPref Prefijo de los nombres en el formulario
     * @param type $arDateType Nombres de campos tipo fecha para convertirlos en fecha de hydra
     * @return array 
     */
    protected function from_post_to_data($arPost=array(),$sPref="det_",$arDateType=array())
    {
        //bug($arPost);
        $arData = array();
        foreach($arPost as $sKey=>$mxValue)
        {
            if($this->is_detail_field($sKey, $sPref)) 
            {
                $sTmpKey = str_replace($sPref, "", $sKey);
                $sTmpValue = trim($mxValue);
                if(in_array($sTmpKey,$arDateType))
                    $sTmpValue = $this->convert_to_hydra_date($mxValue);
                //bug($sTmpKey);
                $arData[$sTmpKey] = $sTmpValue;
            }
        }
        //Limpia los datos de las entidades html y las simples comillas si las tuviera
        ComponentText::clean_before_save_db($arData);
        //bug($arData); die; se limpia
        return $arData;
    }
    
    protected function get_hydra_date()
    {
        return date("YmdHis");
    }
    
    protected function is_error()
    {
        return $this->_is_error;
    }
    
    protected function get_error_message()
    {
        return $this->_error_message;
    }
    
    protected function set_error_message($sMessage)
    {
        $this->_error_message = $sMessage;
    }
    
    protected function set_in_array_error_message($sMessage)
    {
        $this->_arErrorMessage[] = $sMessage;
    }
    
    protected function get_array_error_message()
    {
        return $this->_arErrorMessage;
    }
}