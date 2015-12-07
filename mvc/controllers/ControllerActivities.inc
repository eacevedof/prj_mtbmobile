<?php
class ControllerActivities extends MainController implements InterfaceController  
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
        $this->_code_user = $_POST["Code_User"];
        $this->_code_account = $_POST["Code_Account"];
        //bug($_POST); die;
        $this->load_foreign_links();
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."activities&tab=detail";
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."activities&tab=list";
        $this->_link_url_list_frg = $this->_link_url_list ."&Code_Account=$this->_code_account";
    }
    
    public function basic_list() 
    {
        //Se desea listar contactos de una cuenta
        if(!empty($_POST["Code_Account"])&&empty($_POST["Code_Result"]))
        {
            $this->list_by_account();
        }
        elseif(!empty($_POST["Code_Account"])&&!empty($_POST["Code_Result"]))
        {
            $this->history_by_account();
        }
        //elseif(!empty($_POST["Code_User"]))
        else
        {
            //Por defecto se mostrara las del usuario
            $this->list_by_user();
        }
    }

    //Menu principal. Listado por vendedor con jerarquia
    private function list_by_user()
    {
        //get_mobile_list_by_user se utiliza el usuario en session
        $oActivity = new ModelActivities($this->_session_user_code);
        
        //Datos para paginación. La primera vez list_page=null. Esto lo controla
        //el componente paginate en el modelo
        $oActivity->set_list_page($this->_list_page);
        //Comprobación de busqueda. Por campos
        $arSearchFields = array("DateP","User_Description");
        //Si detecta que no se está buscando lo elimina de la sesion
        //bug($_POST);bug($arSearchFields); die;
        if($this->is_searching($arSearchFields,"oSchUserActivity"))
        {    
            //bug("is searching");
            $sHydraDate = $this->convert_to_hydra_date($_POST["sch_DateP"]);
            $oActivity->set_datep($sHydraDate);
            $oActivity->set_user_description($_POST["sch_User_Description"]);
            $this->oSite->set_in_session("oSchUserActivity", $oActivity);
        }

        //Datos para el listado
        $arDataList = $oActivity->get_mobile_list_by_user();

        $this->_list_num_pages = $oActivity->get_list_num_pages();
        $this->_list_total_regs = $oActivity->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está fuera de rango
        $this->_list_page = $oActivity->get_list_page();
        $arPagesForSelect = $oActivity->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPagesForSelect, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");

        //ActionBar
        $sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar("Actividades");
        
        //Renderizador de listado
        //bug($arDataList); die;

        $oHlpList = new HelperList($arDataList,"Fecha",array("Code"),array("Contacto","Comercial"),
         $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
         "activities","detail",array("Code"));

        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("user_activities.php");
        $this->oView->display();          
    }
    
    private function list_by_account()
    {
        $sCodeAccount = $this->_code_account;
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        $oMtbCliente = new ModelMtbClientes($oClient);
        $oMtbCliente->load_by_code();
        //$oContact = new ModelContacts($this->_session_user_code, null, $oClient);
        $oActivity = new ModelActivities($this->_session_user_code,null,null,$oClient);
        
        //Datos para paginación. La primera vez list_page=null. Esto lo controla
        //el componente paginate en el modelo
        $oActivity->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("DateP","Contact_Description");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientActivity"))
        {    
            //bug("is searching");
            $sHydraDate = $this->convert_to_hydra_date($_POST["sch_DateP"]);
            $oActivity->set_datep($sHydraDate);
            $oActivity->set_contact_description($_POST["sch_Contact_Description"]);
            $this->oSite->set_in_session("oSchClientActivity", $oActivity);
        }

        //Datos para el listado
        $arDataList = $oActivity->get_mobile_list_by_account();

        $this->_list_num_pages = $oActivity->get_list_num_pages();
        $this->_list_total_regs = $oActivity->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oActivity->get_list_page();
        $arPagesForSelect = $oActivity->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPagesForSelect, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("ACTIV.");

        //ActionBar
        $sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar(" ");
        
        //Renderizador de listado
        //bug($arDataList); die;
            
        $oHlpList = new HelperList($arDataList,"Fecha", array("Code"), 
                        array("Contacto","Comercial"),  
                        $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                        "activities","detail",array("Code","Code_Account"));
        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_activities.php");
        $this->oView->display();  
    }
    
    private function history_by_account()
    {
        $sCodeAccount = $_POST["Code_Account"];
        $sCodeResult = $_POST["Code_Result"];
        
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        $oMtbCliente = new ModelMtbClientes($oClient);
        $oMtbCliente->load_by_code();
        $oActivity = new ModelActivities($this->_session_user_code,null,null,$oClient);
        
        //Datos para paginación. La primera vez list_page=null. Esto lo controla
        //el componente paginate en el modelo
        $oActivity->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("DateP","Contact_Description");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientActivity"))
        {    
            //bug("is searching");
            $sHydraDate = $this->convert_to_hydra_date($_POST["sch_DateP"]);
            $oActivity->set_datep($sHydraDate);
            $oActivity->set_contact_description($_POST["sch_Contact_Description"]);
            $this->oSite->set_in_session("oSchClientActivity", $oActivity);
        }

        //Datos para el listado
        $arDataList = $oActivity->get_mobile_list_by_history();

        $this->_list_num_pages = $oActivity->get_list_num_pages();
        $this->_list_total_regs = $oActivity->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oActivity->get_list_page();
        $arPagesForSelect = $oActivity->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPagesForSelect, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("HIST.");

        //ActionBar
        $sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar(" ");
        
        //Renderizador de listado
        //bug($arDataList); die;
       $oHlpList = new HelperList($arDataList,"Fecha", array("Code"), array("Contacto","Comercial"),  
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "activities","detail",array("Code"));

        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_activities_done.php");
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
        $this->oView->add_file("activities_detail.php");
        $this->oView->display();         
    }
    
    private function read_edit()
    {
        if(!$this->_is_error)
        {
            $sCodeActivity = $_POST["Code"];
            $oActivity = new ModelActivities($this->_session_user_code,$sCodeActivity);
            $oActivity->load_by_code();
            //bug($oActivity);
            $oMtbActivity = new ModelMtbActivities($oActivity);
            $oMtbActivity->load_by_code();            
        }
        else
        {
            $sCodeActivity = $this->_oActivity->get_code();
            $oActivity = $this->_oActivity;
            $oMtbActivity = $this->_oMtbActivity;            
        }
        
        //Owner Info Bar
        $sCodeAccount = $oActivity->get_code_account();
        $oClient = new ModelClients(null,$sCodeAccount);
        $oClient->load_by_code();
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription,$sPLnkDetail);
        //si no hay error no hace falta asignar este objeto pq ->oClient se crea con load_by_code
        if($this->_is_error) $oActivity->oClient = $oClient;
        
        //FrgLinks
        //0: Pendiente, 1:Completada 2:replanificada
        $cActivityState = $oActivity->get_code_result();
        if($cActivityState=="0") 
            $sLinkDescription = "ACTIV.";
        else 
            $sLinkDescription = "HIST.";
        
        //Links by account
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description($sLinkDescription);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        
        //ActionBar
        $sPLnkDetail = $this->_link_url_detail."&Code=$sCodeActivity"; 
        $oActionBar = new HelperActionBar("Detalle Actividad","","",$sPLnkDetail); //fix in contact
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        
        //PICKLISTS 
        //Esta carga no se hace en picklists para evitar cargar todos los clientes. Query menos pesada
        $sSQLAnd = "Code='$sCodeAccount'";
        $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd);
        $oSelClient = new HelperSelect($arOptions, "det_Code_Account","Cliente");
        $oSelClient->required();
        $oSelClient->readonly();
        //$oSelClient->set_extras("required");
        $this->oGParams->set_in_vars("oSelClient",$oSelClient);
        
        //Contactos (pasarlo al detalle)
        $sSQLAnd = "Code_Account='$sCodeAccount'";
        $arOptions = MainModel::get_data_for_picklist("contacts", "Code", "Description", $sSQLAnd);
        $oSelContacto = new HelperSelect($arOptions, "det_Code_Contact","Contacto");
        $oSelContacto->required();
        $oSelContacto->readonly();
        $this->oGParams->set_in_vars("oSelContacto",$oSelContacto);    
          
        //Estado, es una sobreescritura a lo que hay en load_picklists. Los datos a cargar dependen
        //de cual sea el estado en la actividad. Si pendiente => se podra seleccionar entre 0 o 1
        //sino entre 1 y 2 completada, replanificada
        if($cActivityState=="0")
            $sSQLAnd = "Type='estado' AND Code IN ('0','1')";
        else
            $sSQLAnd = "Type='estado' AND Code IN ('1','2')";
        $arOptions = MainModel::get_data_for_picklist("activities_array", "Code", "Description", $sSQLAnd);
        $oSelEstado = new HelperSelect($arOptions, "det_Code_Result","Estado");
        $oSelEstado->required();
        $this->oGParams->set_in_vars("oSelEstado",$oSelEstado);
        
        $this->oGParams->set_in_vars("cActivityState", $cActivityState);
        $this->oGParams->set_in_vars("oActivity", $oActivity);
        $this->oGParams->set_in_vars("oMtbActivity", $oMtbActivity);        
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
    }

    /**
     * Si ocurre un error al intentar de dar de alta se pasa los objetos
     * creados desde POST para volverlos a pintar en el detalle
     */
    private function read_new()
    {
        $oActivity = new ModelActivities();
        $oMtbActivity = new ModelMtbActivities($oActivity);
        //No es lo mismo det_Code_Account que Code_Account
        if($this->is_by_account())
        {
            //Cliente dueño de la actividad
            $oClient = new ModelClients(null,$this->_code_account);
            $oClient->load_by_code();
            $sCodeAccount = $oClient->get_code();
            //Owner Bar
            $sPDescription = $sCodeAccount." - ".$oClient->get_name();
            $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
            $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
            $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);

            //FrgLinks
            $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
            $oFrgLink->set_active_description("ACTIV.");
            $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);

            //ActionBar
            //$sLnkSearch = $this->_link_url_list_frg;
            $sLnkRefresh = $this->_link_url_detail."&Code_Account=$sCodeAccount";
            $oActionBar = new HelperActionBar("Nueva Actividad", "","",$sLnkRefresh); 
            
            //Si hay error se recupera desde las variables de clase
            if($this->_is_error)
            {
                $oActivity = $this->_oActivity;
                $oMtbActivity = $this->_oMtbActivity;
            }
            $oActivity->oClient = $oClient;
            
            //Hidden que fuerza el crud desde foreign
            $oHidCodeAccount = new HelperHidden("Code_Account",$sCodeAccount);
            $this->oGParams->set_in_vars("oHidCodeAccount", $oHidCodeAccount);
            
            //PICKLISTS 
            //Esta carga no se hace en picklists para evitar cargar todos los clientes. Query menos pesada
            $sSQLAnd = "Code='$this->_code_account'";
            $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd);
            $oSelClient = new HelperSelect($arOptions, "det_Code_Account","Cliente");
            $oSelClient->required();
            $this->oGParams->set_in_vars("oSelClient",$oSelClient);
        
            //Picklist de contactos
            //Contactos (pasarlo al detalle)
            $sSQLAnd = "Code_Account='$this->_code_account'";
            $arOptions = MainModel::get_data_for_picklist("contacts", "Code", "Description", $sSQLAnd);
            $oSelContacto = new HelperSelect($arOptions, "det_Code_Contact","Contacto");
            $oSelContacto->required();
            $this->oGParams->set_in_vars("oSelContacto",$oSelContacto);                 
            
        }
        //Alta desde otros módulos que no sean cuenta
        else 
        { 
            if($this->_is_error)
            {
                $oActivity = $this->_oActivity;
                $oMtbActivity = $this->_oMtbActivity;
            }
            
            //ActionBar
            $oActionBar = new HelperActionBar("Nueva Actividad", "","",$this->_link_url_detail); 
 
            //PICKLISTS 
            //Clientes
            $sSQLAnd = "Propietario IN (SELECT Code FROM get_code_vendedores('$this->_session_user_code'))";
            $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd);
            $oSelClient = new HelperSelect($arOptions, "det_Code_Account","Cliente");
            $oSelClient->required();
            $this->oGParams->set_in_vars("oSelClient",$oSelClient);
        
            //Picklist de contactos //TODO AJAX
            //Contactos (pasarlo al detalle)
            $arOptions = array(""=>"");
            if($this->_is_error)
            {
                $sCodeAccount = $oActivity->get_code_account();
                $sSQLAnd = "Code_Account='$sCodeAccount'";
                $arOptions = MainModel::get_data_for_picklist("contacts", "Code", "Description", $sSQLAnd);
            }
            $oSelContacto = new HelperSelect($arOptions, "det_Code_Contact","Contacto");
            $oSelContacto->required();
            $this->oGParams->set_in_vars("oSelContacto",$oSelContacto); 
        }
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oActivity", $oActivity);
        $this->oGParams->set_in_vars("oMtbActivity", $oMtbActivity);
    }

    private function save_edit()
    {
        $oActivity = new ModelActivities();
        $oMtbActivity = new ModelMtbActivities($oActivity);
          
        //Creo los arrays de datos
        //bug($_POST); die;
        $arDataActivity = $this->from_post_to_data($_POST,"det_",array("DateP","Date_Next"));
        $arDataMtbActivity = $this->from_post_to_data($_POST,"detm_");

        $arDataMtbActivity["Modify_User"] = $this->_session_user_code;
        $arDataMtbActivity["Modify_Date"] = $this->get_hydra_date();
        $arDataActivity["Modify_User"] = $arDataMtbActivity["Modify_User"];
        $arDataActivity["Modify_Date"] = $arDataMtbActivity["Modify_Date"];
        
        //bug($arDataActivity); bug($arDataMtbActivity); die;
        //Creo la descripcion
        $sCodeActivity = $arDataActivity["Code"];
        $arCondition = array("Code"=>$sCodeActivity);
        $oActivity->auto_update($arDataActivity,$arCondition,array("Code"));

        $oMtbActivity->auto_update($arDataMtbActivity,$arCondition,array("Code"));
        $this->go_to_detail($sCodeActivity);
    }
    
    private function save_replan()
    {
        $oActivity = new ModelActivities();
        $oMtbActivity = new ModelMtbActivities($oActivity);
          
        //========================UPDATE
        $arDataActivity = $this->from_post_to_data($_POST,"det_",array("DateP","Date_Next"));
        $arDataMtbActivity = $this->from_post_to_data($_POST,"detm_");

        $arDataMtbActivity["Modify_User"] = $this->_session_user_code;
        $arDataMtbActivity["Modify_Date"] = $this->get_hydra_date();
        $arDataActivity["Modify_User"] = $arDataMtbActivity["Modify_User"];
        $arDataActivity["Modify_Date"] = $arDataMtbActivity["Modify_Date"];

        $sCodeActivity = $arDataActivity["Code"];
        $arCondition = array("Code"=>$sCodeActivity);
        //update
        $arDataActivity["Code_Result"]="2"; //replanificar
        $oActivity->auto_update($arDataActivity,$arCondition,array("Code"));
        $oMtbActivity->auto_update($arDataMtbActivity,$arCondition,array("Code"));

        //==========================INSERT
        $oActivity->set_code_contact($_POST["det_Code_Contact"]);
        //Lo asigno a datep pq solo me servira para combprobar si ya existe
        $sHydraDateNext = $this->convert_to_hydra_date($_POST["det_Date_Next"]);
        $oActivity->set_datep($sHydraDateNext);
        $oActivity->set_code_newtype($_POST["det_Code_Newtype_Next"]);
        
        $arDataActivity = array(); $arDataMtbActivity = array();
        $arDataActivity["Code_User"] = $_POST["det_Code_User"];
        $arDataActivity["Code_Account"] = $_POST["det_Code_Account"];
        $arDataActivity["Code_Contact"] = $_POST["det_Code_Contact"];
        $arDataActivity["Code_Result"] = "0"; //Pendiente
        $arDataActivity["DateP"] = $_POST["det_Date_Next"];
        $arDataActivity["Code_Newtype"] = $_POST["det_Code_Newtype_Next"];
        $arDataActivity["Notes"] = $_POST["det_Notas_Centralita"];
        
        //Creo los arrays de datos
        $arDataActivity["Create_User"] = $this->_session_user_code;
        $arDataActivity["Create_Date"] = $this->get_hydra_date();
        $arDataMtbActivity["Create_User"] = $arDataActivity["Create_User"];
        $arDataMtbActivity["Create_Date"] = $arDataActivity["Create_Date"];  
         
        $arDataActivity["Modify_User"] = $arDataActivity["Create_User"];
        $arDataActivity["Modify_Date"] = $arDataActivity["Create_Date"];         
        $arDataMtbActivity["Modify_User"] = $arDataActivity["Create_User"];
        $arDataMtbActivity["Modify_Date"] = $arDataActivity["Create_Date"]; 
        
        //bug($arDataActivity); bug($arDataMtbActivity); //die;
        $sCodeActivity = $oActivity->get_key_by_fields();
        $oActivity->set_keys_and_values(array("Code"=>$sCodeActivity));
        $oActivity->auto_insert($arDataActivity);
        $arKeysAndValues = $oActivity->get_keys_and_values();
        $oMtbActivity = new ModelMtbActivities($oActivity);
        $oMtbActivity->set_keys_and_values($arKeysAndValues);
        $oMtbActivity->auto_insert($arDataMtbActivity);
        
        $this->_succes_message = "Actividad Replanificada";
        $this->oSite->set_in_session("sSuccesMessage", $this->_succes_message);
        $this->go_to_detail($sCodeActivity);        
    }    
    
    private function save_new() 
    { 
        $oActivity = new ModelActivities();
        //Datos para construir key
        $oActivity->set_code_contact($_POST["det_Code_Contact"]);
        $sHydraDate = $this->convert_to_hydra_date($_POST["det_DateP"]);
        $oActivity->set_datep($sHydraDate);
        $oActivity->set_code_newtype($_POST["det_Code_Newtype"]);
        
        //Creo los arrays de datos
        $arDataActivity = $this->from_post_to_data($_POST,"det_",array("DateP"));
        $arDataMtbActivity = $this->from_post_to_data($_POST,"detm_");
        
        $arDataActivity["Create_User"] = $this->_session_user_code;
        $arDataActivity["Create_Date"] = $this->get_hydra_date();
        $arDataMtbActivity["Create_User"] = $arDataActivity["Create_User"];
        $arDataMtbActivity["Create_Date"] = $arDataActivity["Create_Date"];  
         
        $arDataActivity["Modify_User"] = $arDataActivity["Create_User"];
        $arDataActivity["Modify_Date"] = $arDataActivity["Create_Date"];         
        $arDataMtbActivity["Modify_User"] = $arDataActivity["Create_User"];
        $arDataMtbActivity["Modify_Date"] = $arDataActivity["Create_Date"]; 
    
        $sCodeActivity = $oActivity->get_key_by_fields();
        $oActivity->set_keys_and_values(array("Code"=>$sCodeActivity));
        $oActivity->auto_insert($arDataActivity);
        $arKeysAndValues = $oActivity->get_keys_and_values();
        $oMtbActivity = new ModelMtbActivities($oActivity);
        $oMtbActivity->set_keys_and_values($arKeysAndValues);
        $oMtbActivity->auto_insert($arDataMtbActivity);
        $this->go_to_detail($sCodeActivity);
    }
    
    private function is_error_for_new()
    {
        $dToday = date("Ymd");
        $oActivity = new ModelActivities();
        $oActivity->set_code_user($_POST["det_Code_User"]);
        //Datos para construir key
        $oActivity->set_code_contact($_POST["det_Code_Contact"]);
        $sHydraDate = $this->convert_to_hydra_date($_POST["det_DateP"]);
        $oActivity->set_datep($sHydraDate);
        $oActivity->set_code_newtype($_POST["det_Code_Newtype"]);
        $sCodeActivity = $oActivity->get_key_by_fields();
        
        //bug((((int)$dToday)<((int)$sHydraDate))); die;
        $dToday = (int)$dToday; $sHydraDate = (int)$sHydraDate;
        
        //bug($dToday,"today"); bug($sHydraDate,"hydradate"); 
        if($dToday>$sHydraDate)
        {
            $this->_is_error = true;
            $this->_error_message = "La fecha debe ser posterior a la actual";
        }
        //Si ya existe este registro
        elseif($oActivity->is_key_in_table())        
        {
            $this->_is_error = true;
            $this->_error_message = "Ya existe una actividad para este contacto, acción y fecha";
        }
        elseif($oActivity->get_code_user()=="")
        {
            $this->_is_error = true;
            $this->_error_message = "Debe proporcionar un asesor";
        }
        elseif($oActivity->get_code_contact()=="")
        {
            $this->_is_error = true;
            $this->_error_message = "Debe seleccionar un contacto";
        }
        elseif($oActivity->get_code_newtype()=="")
        {    
            $this->_is_error = true;
            $this->_error_message = "Debe seleccionar una acción";
        }
        
        if($this->_is_error)
        {
            $this->oGParams->set_in_vars("sErrorMessage", $this->_error_message);
            //Creo los objetos y los guardo en la clase
            $this->build_objects_from_post();
            //Cargo los combos
            $this->load_detail_picklists();
        }
        return $this->_is_error;
    }
    
    private function is_error_for_edit()
    {
        $arMustEmpty[] = $_POST["det_Date_Next"];
        $arMustEmpty[] = $_POST["det_Code_Newtype_Next"];
        $arMustEmpty[] = $_POST["det_Notas_Centralita"];
        
        //bug($dToday,"today"); bug($sHydraDate,"hydradate"); 
        $this->_error_message = "La modificación no se ha llevado a cabo. ";
        
        foreach($arMustEmpty as $sValue)
        {
            if(!empty($sValue))
            {
                $this->_is_error = true;
                $this->_error_message .= "Ha rellenado campos de replanificación. Debe utilizar el botón \"Replanificar\"";
                break;
            }
        }

        if($this->_is_error)
        {
            $this->oGParams->set_in_vars("sErrorMessage", $this->_error_message);
            //Creo los objetos y los guardo en la clase
            $this->build_objects_from_post();
            //Cargo los combos
            $this->load_detail_picklists();
        }
        return $this->_is_error;         
    }
    
    private function is_error_for_replan()
    {
        $dToday = date("Ymd");
        $sOldHydraDate = (int)$this->convert_to_hydra_date($_POST["det_DateP"]);
        
        $oActivity = new ModelActivities();
        $oActivity->set_code_user("det_Code_User");
        $oActivity->set_code_contact($_POST["det_Code_Contact"]);
        
        $sHydraDateNext = $this->convert_to_hydra_date($_POST["det_Date_Next"]);
        //Lo asigno a datep pq solo me servira para combprobar si ya existe
        $oActivity->set_datep($sHydraDateNext);
        //Lo asigno a newtype por lo anterior
        $oActivity->set_code_newtype($_POST["det_Code_Newtype_Next"]);
        $sCodeActivity = $oActivity->get_key_by_fields();
        
        //bug((((int)$dToday)<((int)$sHydraDate))); die;
        $dToday = (int)$dToday; $sHydraDateNext = (int)$sHydraDateNext;
        
        //bug($dToday,"today"); bug($sHydraDate,"hydradate"); 
        $this->_error_message = "La replanificación no se ha llevado a cabo. ";
        if($sHydraDateNext<$dToday || $sHydraDateNext<=$sOldHydraDate)
        {
            $this->_is_error = true;
            $this->_error_message .= "La fecha de replanificación debe ser posterior a la actual";
        }
        //Si ya existe este registro
        elseif($oActivity->is_key_in_table())        
        {
            $this->_is_error = true;
            $this->_error_message .= "Ya existe una actividad para este contacto, acción y fecha";
        }
        elseif($oActivity->get_code_user()=="")
        {
            $this->_is_error = true;
            $this->_error_message .= "Debe proporcionar un asesor";
        }
        elseif($oActivity->get_code_contact()=="")
        {
            $this->_is_error = true;
            $this->_error_message .= "Debe seleccionar un contacto";
        }
        elseif($oActivity->get_code_newtype()=="")
        {
            $this->_is_error = true;
            $this->_error_message .= "Debe seleccionar una acción";
        }
        
        if($this->_is_error)
        {
            $this->oGParams->set_in_vars("sErrorMessage", $this->_error_message);
            //Creo los objetos y los guardo en la clase
            $this->build_objects_from_post();
            //Cargo los combos
            $this->load_detail_picklists();
        }
        return $this->_is_error;        
    }
    
    /**
     * En caso de error hay que reconstruir los objetos de tipo modelo
     * que intervienen en el formulario desde el array post para volverlos 
     * a mostrar en el formulario
     */
    private function build_objects_from_post()
    {
        $oActivity = new ModelActivities();
        $oActivity->set_code($_POST["det_Code"]);
        $oActivity->set_code_user($_POST["det_Code_User"]);
        $oActivity->set_code_account($_POST["det_Code_Account"]);
        $oActivity->set_code_contact($_POST["det_Code_Contact"]);
        $oActivity->set_code_newtype($_POST["det_Code_Newtype"]);
        $oActivity->set_datep($_POST["det_DateP"]);
        $oActivity->set_start_time($_POST["det_Start_Time"]);
        $oActivity->set_end_time($_POST["det_End_Time"]);
        $oActivity->set_notes($_POST["det_Notes"]);
        $oActivity->set_code_result($_POST["det_Code_Result"]);
        //REPLANIFICAR
        $oActivity->set_date_next($_POST["det_Date_Next"]);
        $oActivity->set_code_newtype_next($_POST["det_Code_Newtype_Next"]);
        $oActivity->set_notas_centralita($_POST["det_Notas_Centralita"]);
        
        $oMtbActivity = new ModelMtbActivities($oActivity);
        $oMtbActivity->set_resultado($_POST["detm_Resultado"]);
        
        $this->_oActivity = $oActivity;
        $this->_oMtbActivity = $oMtbActivity;
    }
    
    private function load_detail_picklists()
    {
        //Vendedores
        $sSQLAnd = "Code IN (SELECT Code FROM get_code_vendedores('$this->_session_user_code'))";
        $arOptions = MainModel::get_data_for_picklist("core_users","Code","Description",$sSQLAnd);
        $oSelUser = new HelperSelect($arOptions, "det_Code_User","Comercial");
        $oSelUser->required();
        $this->oGParams->set_in_vars("oSelUser",$oSelUser);
        
        //Clientes se carga en read_new o read_edit para evitar el exceso de datos cargados
        //desde un módulo externo
        /*
        $sSQLAnd = "Code IN (SELECT Code FROM clients WHERE Propietario 
                    IN (SELECT Code FROM get_code_vendedores('$this->_session_user_code'))) 
                    ";
        $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd);
        $oSelClient = new HelperSelect($arOptions, "det_Code_Account");
        $oSelClient->set_label("Cliente");
        $oSelClient->set_extras_for_label("class=\"clsRequired\"");
        $oSelClient->set_extras("required");
        $this->oGParams->set_in_vars("oSelClient",$oSelClient);
         * 
         */
        
        //Accion
        $sSQLAnd = "Type='newtypes'";
        $arOptions = MainModel::get_data_for_picklist("activities_array", "Code", "Description", $sSQLAnd);
        $oSelAccion = new HelperSelect($arOptions, "det_Code_Newtype","Acción");
        $oSelAccion->required();
        $this->oGParams->set_in_vars("oSelAccion",$oSelAccion);
        
        //REPLANIFICAR - Proxima accon
        $oSelProximaAccion = new HelperSelect($arOptions, "det_Code_Newtype_Next","Próxima Acción");
        $oSelProximaAccion->set_class("clsRequired");
        //bug($oSelProximaAccion); die;
        $this->oGParams->set_in_vars("oSelProximaAccion",$oSelProximaAccion);        
        
        //Hora Inicio
        $sSQLAnd = "Type='horas'";
        $arOptions = MainModel::get_data_for_picklist("activities_array", "Code", "Description", $sSQLAnd, "Code");
        ksort($arOptions,SORT_NUMERIC);
        $oSelHoraInicio = new HelperSelect($arOptions, "det_Start_Time","Hora Inicio");
        $this->oGParams->set_in_vars("oSelHoraInicio",$oSelHoraInicio);
        
        //Hora Final
        $oSelHoraFinal = new HelperSelect($arOptions, "det_End_Time","Hora Final");
        $this->oGParams->set_in_vars("oSelHoraFinal",$oSelHoraFinal);

        //Estado
        $sSQLAnd = "Type='estado' AND Code='0'";
        $arOptions = MainModel::get_data_for_picklist("activities_array", "Code", "Description", $sSQLAnd);
        $oSelEstado = new HelperSelect($arOptions, "det_Code_Result","Estado");
        $oSelEstado->required();
        $this->oGParams->set_in_vars("oSelEstado",$oSelEstado);

        //Fecha  FECHAS NO, SOLO PICKLISTS
        /* 
        $sFechaHoy = $this->oSite->get_today_date();
        $oDateFecha = new HelperDate("det_DateP", $sFechaHoy,"Fecha");
        $oDateFecha->required();
        $this->oGParams->set_in_vars("oDateFecha",$oDateFecha);
        
        $oDateFechaProxima = new HelperDate("det_Date_Next", $sFechaHoy,"Fecha Próxima");
        $oDateFechaProxima->set_class("clsRequired");
        //$oDateFechaProxima->required();
        $this->oGParams->set_in_vars("oDateFechaProxima",$oDateFechaProxima);*/
        
        /*
        //Contactos (se carga en el read)
        $sSQLAnd = "Code_Account='$this->_code_account'";
        $arOptions = MainModel::get_data_for_picklist("contacts", "Code", "Description", $sSQLAnd);
        $oSelContacto = new HelperSelect($arOptions, "det_Code_Newtype");
        $oSelContacto->set_label("Contacto");
        $this->oGParams->set_in_vars("oSelContacto",$oSelContacto);        
        */
        
        //Sirve para ejecutar onready
        $oHlpJavascript = new HelperJavascript();
        $arRequired = array("det_Code_User","det_Code_Account","det_Code_Contact",
            "det_Code_Newtype","det_Code_Result", "det_DateP","det_Code_Newtype_Next","det_Date_Next");
        $oHlpJavascript->set_required_fields($arRequired);
        $this->oGParams->set_in_vars("oHlpJavascript", $oHlpJavascript);
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
    
    private function is_replanning()
    {
        return $_POST["action"]=="replan";
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
        /*
        $sCodeUser = $this->_session_user_code;
        $sCodeContact = $_POST["det_Code"];
        $sCodeAccount = $_POST["det_Code_Account"];
        //Se le pasa el usuario para comprobar que sea un contacto que le pertenezca
        $oContact = new ModelContacts($sCodeUser, $sCodeContact);
        if($oContact->belongs_to_user())
        {    
            $arKeysAndValues = array("Code"=>$sCodeContact);
            $oMtbContacto = new ModelMtbContactos($oContact);
            $oMtbContacto->set_keys_and_values($arKeysAndValues);
            $oContact->set_keys_and_values($arKeysAndValues);
            $oMtbContacto->auto_delete();
            $oContact->auto_delete();
            $this->go_to_list_by_account($sCodeAccount);
        }
        else
        {   
            $sMessage = "No tiene permisos para eliminar este contacto";
            $this->oGParams->set_message($sMessage);
            $this->go_to_detail($sCodeContact);
        }*/
    }	
}
