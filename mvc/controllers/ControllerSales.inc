<?php
class ControllerSales extends MainController implements InterfaceController  
{
    private $_code_user;
    private $_code_account;
    private $_link_url_list_frg = "";
    
    public function __construct()
    {
        parent::__construct();
        $this->_list_page = $_POST["hidNumPage"];
        $this->_code_account = $_POST["Code_Account"];
        //bug($_POST); die;
        $this->load_foreign_links();
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."ventas&tab=detail";
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."ventas&tab=list";
        $this->_link_url_list_frg = $this->_link_url_list ."&Code_Account=$this->_code_account";
    }
    
    public function basic_list() 
    {
        if(!empty($_POST["Code_Account"]))
        {
            $this->list_by_account();
        }
        else
        {}
    }
    
    private function list_by_account()
    {
        $sCodeAccount = $this->_code_account;
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        
        $oMtbEstadistica = new ModelMtbEstadisticas();
        //$oMtbEstadistica->set_code_agent($this->_session_user_code);
        $oMtbEstadistica->set_code_agent($oClient->get_propietario());
        $oMtbEstadistica->set_code_account($sCodeAccount);
        $oMtbEstadistica->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Meses","Code_Familia","Code_Subfamilia");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientStadistics"))
        {   
            //bug(empty($_POST["sch_Meses"]),"meses vacio"); 
            $oMtbEstadistica->set_meses($_POST["sch_Meses"]);
            $oMtbEstadistica->set_code_familia($_POST["sch_Code_Familia"]);
            $oMtbEstadistica->set_code_subfamilia($_POST["sch_Code_Subfamilia"]);
            $this->oSite->set_in_session("oSchClientStadistics", $oMtbEstadistica);
        }

        //Datos para el listado
        $arDataList = $oMtbEstadistica->get_mobile_list_by_account();
        
        $this->_list_num_pages = $oMtbEstadistica->get_list_num_pages();
        $this->_list_total_regs = $oMtbEstadistica->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oMtbEstadistica->get_list_page();
        $arPages = $oMtbEstadistica->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPages, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("VENTAS");

        //ActionBar
        //$sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar(" ");
/*   ["Id"]=>
    string(4) "--01"
    ["Code_Familia"]=>
    string(0) ""
    ["Familia_Description"]=>
    string(0) ""
    ["Code_SubFamilia"]=>
    string(0) ""
    ["Subfamilia_Description"]=>
    string(0) ""
    ["Mes"]=>
    string(2) "01"
    ["Mes_Description"]=>
    string(5) "Enero"
    ["VentasAct"]=>
    string(5) "0.000"
    ["VentasAnt"]=>
    string(5) "0.000"
 HelperList($arDataList,"Fecha",array("Code"),array("Contacto","Comercial"),
         $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
         "activities","detail",array("Code"));
 */
        
        $oHlpList = new HelperList($arDataList,"", array(""), 
                array("Familia_Description","Subfamilia_Description","Mes_Description","VentasAct","VentasAnt"), 
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs);
        $oHlpList->anchor_nohref();
        $oHlpList->set_header_labels($oMtbEstadistica->get_dinamic_labels());
        //bug($arDataList); die;
        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_sales.php");
        $this->oView->display();  
    }
     
    public function detail()
    {
        //Escribiendo
        //action=save
        if($this->is_saving())
        {
            if($this->is_new())
            {
                if(!$this->is_error_for_new())
                    $this->save_new();
                else
                    $this->read_new();
            }
            else
            {
                if(!$this->is_error_for_edit())
                    $this->save_edit();
                else
                    //si hay error los objetos ya están en la variable de clase
                    $this->read_edit();
            }            
        }
        //action=replan
        elseif($this->is_replanning())
        {
            if(!$this->is_error_for_replan())
                $this->save_replan();
            else
                //Readedit necesita un codigo para crear los objetos
                //si hay error los objetos ya están en la variable de clase
                $this->read_edit();
        }
        //action=delete
        elseif($this->is_deleting())
        {
            $this->delete();
        }
        //Leyendo
        else
        {
            $this->load_detail_picklists();
            if($this->is_new())
            {
                $this->read_new();
            }
            //Existente
            else
            {
                $this->read_edit();
            }
        }
        $this->oView->add_file("discounts_detail.php");
        $this->oView->display();         
    }
    
    private function read_edit()
    {
      //tomar ejemplo de activities
    }

    /**
     * Si ocurre un error al intentar de dar de alta se pasa los objetos
     * creados desde POST para volverlos a pintar en el detalle
     */
    private function read_new()
    {
        //tomar ejemplo de activities
    }

    private function save_edit()
    {
        //tomar ejemplo de activities
    }
    
    private function save_new() 
    { 
        //tomar ejemplo de activities
    }
    
    private function is_error_for_new()
    {
       //tomar ejemplo de activities
    }
    
    private function is_error_for_edit()
    {
       //tomar ejemplo de activities     
    }
    
    /**
     * En caso de error hay que reconstruir los objetos de tipo modelo
     * que intervienen en el formulario desde el array post para volverlos 
     * a mostrar en el formulario
     */
    private function build_objects_from_post()
    {
        //tomar ejemplo de activities
    }
    
    private function load_detail_picklists()
    {
        //tomar ejemplo de activities
    }

    private function is_new()
    {
        //bug($_POST); die;
        if(empty($_POST["Code"])&&empty($_POST["det_Code"]))
        {
            //bug($_POST["Code"]); bug($_POST["det_Code"]);
            $isNew = true;
        }
        else
        {
            $isNew = false;
        }   
        $this->oGParams->set_in_vars("isNew", $isNew);
        //bug($isNew,"isnew");
        return $isNew;
    }
    
    private function is_by_account()
    {
        if(!empty($this->_code_account))
        {    
            $this->oGParams->set_in_vars("isByAccount", true); 
            return true; 
        } 
        $this->oGParams->set_in_vars("isByAccount", false);
        return false;
    }

    protected function is_deleting()
    {
        return $_POST["action"]=="delete";
    }    

    public function go_to_detail($sCodeActivity)
    {
        $_POST = null; $_GET = null;
        header("Location: $this->_link_url_detail&Code=$sCodeActivity");
        //header("Location: ".JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=contacts&tab=detail&Code=$sCodeContact");
        exit();
    }
    
    private function go_to_list_by_account($sCodeAccount)
    {
        $_POST = null; $_GET = null;
        //header("Location: ".JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=contacts&tab=list&Code_Account=$sCodeAccount");
        header("Location: $this->_link_url_list_frg$sCodeAccount");
        exit();
    }

    public function delete()
    {
        //copiar de contactos
    }	
}
