<?php
class ControllerDiscounts extends MainController implements InterfaceController  
{
    private $_code_user;
    private $_code_account;
    private $_link_url_list_frg = "";
    
    //Para la recarga en caso de error
    private $_oActivity;
    private $_oMtbActivity;
    //private $_isError = false;


    public function __construct()
    {
        parent::__construct();
        $this->_list_page = $_POST["hidNumPage"];
        $this->_code_account = $_POST["Code_Account"];
        //bug($_POST); die;
        $this->load_foreign_links();
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."descuentos&tab=detail";
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."descuentos&tab=list";
        $this->_link_url_list_frg = $this->_link_url_list ."&Code_Account=$this->_code_account";
    }
    
    public function basic_list() 
    {
        //bug($_POST); die;
        //Se desea listar contactos de una cuenta
        if(!empty($_POST["Code_Account"]) && $_POST["Type"]=="subfamilia")
        {
            $this->list_subfamily_by_account();
        }
        elseif(!empty($_POST["Code_Account"]) && $_POST["Type"]=="neto")
        {
            //Por defecto se mostrara las del usuario
            $this->list_neto_by_account();
        }
        elseif(!empty($_POST["Code_Account"]))
        {
            $this->list_family_by_account();
        }
        else
        {}
    }
    
    private function list_subfamily_by_account()
    {
        //bug($_POST);
        $sCodeAccount = $_POST["Code_Account"];
       
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        
        $oDiscount = new ModelMtbDescuentos($this->_session_user_code);
        $oDiscount->set_code_cliente($sCodeAccount);
        $oDiscount->set_code_familia($_POST["Code_Familia"]);
        $oDiscount->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Subfamilia_Description");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientDiscount"))
        {    
            $oDiscount->set_descripcion_subfamilia($_POST["sch_Subfamilia_Description"]);
            $this->oSite->set_in_session("oSchClientDiscount", $oDiscount);
        }

        //Datos para el listado
        $arDataList = $oDiscount->get_mobile_list_subfamily_by_account();

        $this->_list_num_pages = $oDiscount->get_list_num_pages();
        $this->_list_total_regs = $oDiscount->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oDiscount->get_list_page();
        $arPages = $oDiscount->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPages, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("CONDIC.");

        //ActionBar
        $oActionBar = new HelperActionBar("Dto. Subfamilia");

        //bug($arDataList);die;
        $oHlpList = new HelperList($arDataList,"Subfamilia_Description",array("Code_SubFamilia"), array("Dto1"),  
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "propuestas","detail",
                array("Code_Account","Code_SubFamilia","Subfamilia_Description","Code_Familia","Familia_Description","Type","Dto1"));
        $oHlpList->use_tiptext_in_p();
        $oHlpList->use_tiptext_in_h3();
        
        //Variables a utilizar en la vista 
        //Parametros
        $this->oGParams->set_in_vars("sCodeAccount", $_POST["Code_Account"]);
        $this->oGParams->set_in_vars("sCodeFamilia", $_POST["Code_Familia"]);
        $this->oGParams->set_in_vars("sFamiliaDescription", $_POST["Familia_Description"]);
        
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_discounts_subfamily.php");
        $this->oView->display(); 
    }
    
    private function list_neto_by_account()
    {
        //bug($_POST); die;
        $sCodeAccount = $_POST["Code_Account"];
       
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        
        $oDiscount = new ModelMtbDescuentos();
        $oDiscount->set_code_cliente($sCodeAccount);
        $oDiscount->set_code_familia($_POST["Code_Familia"]);
        
        $oDiscount->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Code_Articulo","Producto_Description");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientDiscount"))
        {    
            $oDiscount->set_code_articulo($_POST["sch_Code_Articulo"]);
            $oDiscount->set_descripcion_producto($_POST["sch_Producto_Description"]);
            $this->oSite->set_in_session("oSchClientDiscount", $oDiscount);
        }

        //Datos para el listado
        $arDataList = $oDiscount->get_mobile_list_neto_by_account();
        //bug($arDataList); die;
        $this->_list_num_pages = $oDiscount->get_list_num_pages();
        $this->_list_total_regs = $oDiscount->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oDiscount->get_list_page();
        $arPages = $oDiscount->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPages, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sOwnDescrip = $sCodeAccount." - ".$oClient->get_name();
        $sOwnLinkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sOwnDescrip, $sOwnLinkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("CONDIC.");

        //ActionBar
        $oActionBar = new HelperActionBar("Dto. Producto");

        //bug($arDataList);die;
        $oHlpList = new HelperList($arDataList,"Producto_Description",array("Code_Articulo"), 
                array("Subfamilia_Description","Cantidad","Precio"),  
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "propuestas","detail",
                array("Code_Account","Code_Familia","Familia_Description","Code_SubFamilia",
                "Subfamilia_Description","Code_Articulo","Producto_Description","Precio","Cantidad","Type"));

        $oHlpList->use_tiptext_in_p();
        $oHlpList->use_tiptext_in_h3();
        //Variables a utilizar en la vista 
        //Parametros
        $this->oGParams->set_in_vars("sCodeAccount", $_POST["Code_Account"]);
        $this->oGParams->set_in_vars("sCodeFamilia", $_POST["Code_Familia"]);
        $this->oGParams->set_in_vars("sFamiliaDescription", $_POST["Familia_Description"]);
        //$this->oGParams->set_in_vars("sFamiliaDescription", $_POST["Familia_Description"]);
        
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_discounts_neto.php");
        $this->oView->display(); 
    }
    
    private function list_family_by_account()
    {
        $sCodeAccount = $this->_code_account;
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        
        $oDiscount = new ModelMtbDescuentos($this->_session_user_code);
        $oDiscount->set_code_cliente($sCodeAccount);
        $oDiscount->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Code_Articulo","Code_Familia","Code_Subfamilia");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientDiscount"))
        {    
            $oDiscount->set_code_articulo($_POST["sch_Code_Articulo"]);
            $oDiscount->set_code_familia($_POST["sch_Code_Familia"]);
            $oDiscount->set_code_subfamilia($_POST["sch_Code_Subfamilia"]);
            $this->oSite->set_in_session("oSchClientDiscount", $oDiscount);
        }

        //Datos para el listado
        $arDataList = $oDiscount->get_mobile_list_family_by_account();

        $this->_list_num_pages = $oDiscount->get_list_num_pages();
        $this->_list_total_regs = $oDiscount->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oDiscount->get_list_page();
        $arPages = $oDiscount->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPages, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("CONDIC.");

        //ActionBar
        //$sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar("Dto. Familia");

        $oHlpList = new HelperListPopup("propuestas", $arDataList, array("Code_Familia"), 
                array("DtoFamilia","DtoSubfamilia","Is_Neto"), "Familia_Description", 
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs);

        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_discounts_family.php");
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
                //Simple o desde cuenta
                //Debug::set_message("Read new");
                $this->read_new();
            }
            //Existente
            else
            {
                //Debug::set_message("Read old");
                //$sCodeActivity = $_POST["Code"];
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
