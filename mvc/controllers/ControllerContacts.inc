<?php
require_once "ControllerClients.inc";

class ControllerContacts extends MainController implements InterfaceController  
{
    private $_code_account;
    private $_link_url_list_frg = "";
    
    public function __construct()
    {
        parent::__construct();
        $this->_list_page = $_POST["hidNumPage"];
        $this->_code_account = $_POST["Code_Account"];
        $this->load_foreign_links();
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."contacts&tab=list";
        $this->_link_url_list_frg = $this->_link_url_list ."&Code_Account=$this->_code_account";
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."contacts&tab=detail";
    }
    
    public function basic_list() 
    {
        //Se desea listar contactos de una cuenta
        if(!empty($_POST["Code_Account"]))
        {
            $this->list_by_account();
        }
    }
    
    private function list_by_account()
    {
        $sCodeAccount = $this->_code_account;
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        $oMtbCliente = new ModelMtbClientes($oClient);
        $oMtbCliente->load_by_code();
        $oContact = new ModelContacts($this->_session_user_code, null, $oClient);

        //Datos para paginación. La primera vez list_page=null. Esto lo controla
        //el componente paginate en el modelo
        $oContact->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Full_Name","Cargo");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oContactSearch"))
        {    
            //bug("is searching");
            $oContact->set_first_name($_POST["sch_Full_Name"]);
            $oContact->set_last_name($_POST["sch_Full_Name"]);
            $oContact->set_mtb_last_name2($_POST["sch_Full_Name"]);
            //lleno el atributo auxiliar para completar el formulario de busqueda
            $oContact->set_full_name($_POST["sch_Full_Name"]);
            $oContact->set_cargo($_POST["sch_Cargo"]);
            $this->oSite->set_in_session("oContactSearch", $oContact);
        }

        //Datos para el listado
        $arDataList = $oContact->get_mobile_list_by_account();

        $this->_list_num_pages = $oContact->get_list_num_pages();
        $this->_list_total_regs = $oContact->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oContact->get_list_page();
        $arPagesForSelect = $oContact->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPagesForSelect, "selPage", "", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this)");

        //Owner Bar
        $sPDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oHlpOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);
        
        //FrgLinks
        $oHlpFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oHlpFrgLink->set_active_module("contacts");

        //ActionBar
        $sLnkSearch = $this->_link_url_list_frg;
        $oHlpActionBar = new HelperActionBar(" ");
        
        //Renderizador de listado
        //bug($arDataList);
        $oHlpList = new HelperList($arDataList,"Full_Name",array("Code"),array("Telefono","Cargo"),  
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "contacts","detail",array("Code"));

        //Variables a utilizar en la vista $oHlpList
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oHlpOwnerInfoBar", $oHlpOwnerInfoBar);
        $this->oGParams->set_in_vars("oHlpFrgLink", $oHlpFrgLink);
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_contacts.php");
        $this->oView->display();  
    }
    
    public function detail()
    {
        //Escribiendo
        if($this->is_saving())
        {
            if($this->is_new())
            {
                //Debug::set_message("save new");
                $this->save_new();
            }
            else
            {
                //Debug::set_message("save old");
                $this->save_edit();
            }            
        }
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
            else
            {
                //Debug::set_message("Read old");
                $sCodeContact = $_POST["Code"];
                $this->read_edit($sCodeContact);
            }
        }
        $this->oView->add_file("contacts_detail.php");
        $this->oView->display();         
    }
    
    private function read_edit($sCodeContact)
    {
        //Objetos
        $oContact = new ModelContacts($this->_session_user_code, $sCodeContact);
        $oContact->load_by_code();
        
        $oMtbContacto = new ModelMtbContactos($oContact);
        $oMtbContacto->load_by_code();
        
        /*
        $sH1 = $oContact->get_code()." ".$oContact->get_first_name();
        $oHlpHeaderBar = new HelperHeaderBar();
        $sBackLink = JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=contacts&tab=list&Code_Account=$sCodeAccount";
        $oHlpHeaderBar->set_h1_text($sH1);
        $oHlpHeaderBar->set_p_text("Contacto");
        $oHlpHeaderBar->set_back_link($sBackLink);        */
        //Owner Bar
        $sCodeAccount = $oContact->oClient->get_code();
        $sPDescription = $sCodeAccount." - ".$oContact->oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oHlpOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription,$sPLnkDetail);
        
        //FrgLinks
        $oHlpFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oHlpFrgLink->set_active_module("contacts");

        //ActionBar
        $sPLnkDetail = $this->_link_url_detail."&Code=$sCodeContact";
        $oHlpActionBar = new HelperActionBar("Detalle Contacto","",$sLnkSearch,$sPLnkDetail);

        //Picklist desde cliente
        $sSQLAnd = "Code='$sCodeAccount'";
        $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd,"","",false);
        $oSelClients = new HelperSelect($arOptions, "det_Code_Account");
        $oSelClients->set_label("Cliente");
        $oSelClients->readonly();
        $oSelClients->required();
        $this->oGParams->set_in_vars("oSelClients",$oSelClients);    

        $this->oGParams->set_in_vars("oContact", $oContact);
        $this->oGParams->set_in_vars("oMtbContacto", $oMtbContacto);        
        $this->oGParams->set_in_vars("oHlpOwnerInfoBar", $oHlpOwnerInfoBar);
        $this->oGParams->set_in_vars("oHlpFrgLink", $oHlpFrgLink);
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);
    }

    private function read_new()
    {
        if($this->is_by_account())
        {
            $oClient = new ModelClients($this->_session_user_code,$this->_code_account);
            $oClient->load_by_code();
            $oMtbCliente = new ModelMtbClientes($oClient);
            
            //Owner Bar
            $sCodeAccount = $oClient->get_code();
            $sPDescription = $sCodeAccount." - ".$oClient->get_name();
            $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
            $oHlpOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription, $sPLnkDetail);

            //FrgLinks
            $oHlpFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
            $oHlpFrgLink->set_active_module("contacts");

            //ActionBar
            //$sLnkSearch = $this->_link_url_list_frg;
            $sLnkRefresh = $this->_link_url_detail."&Code_Account=$sCodeAccount";
            $oHlpActionBar = new HelperActionBar("Nuevo Contacto", "","",$sLnkRefresh); 
            
            /*
            $sSQLAnd = "Code IN (SELECT Code FROM clients WHERE Propietario 
                        IN (SELECT Code FROM get_code_vendedores('$this->_session_user_code')))";
             */
            $sSQLAnd = "Code='$sCodeAccount'";
            $arOptions = MainModel::get_data_for_picklist("clients", "Code", "Name", $sSQLAnd);
            $oSelClients = new HelperSelect($arOptions, "det_Code_Account");
            $oSelClients->set_label("Cliente");
            $oSelClients->required();
            $oSelClients->readonly();
            $this->oGParams->set_in_vars("oSelClients",$oSelClients);            
            
        }    
        //otro módulo ó menu general
        else{}

        $oContact = new ModelContacts($this->_session_user_code, null, $oClient);
        $oMtbContact = new ModelMtbContactos($oContact);

        $this->oGParams->set_in_vars("oContact", $oContact);
        $this->oGParams->set_in_vars("oMtbContact", $oMtbContact);
        $this->oGParams->set_in_vars("oHlpOwnerInfoBar", $oHlpOwnerInfoBar);
        $this->oGParams->set_in_vars("oHlpFrgLink", $oHlpFrgLink);
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);        
    }

    private function save_edit()
    {
        $oContact = new ModelContacts(null);
        $oMtbContacto = new ModelMtbContactos($oContact);
        
        $oContact->set_first_name($_POST["det_First_Name"]);
        $oContact->set_last_name($_POST["det_Last_Name"]);
        $oContact->set_cargo($_POST["det_Cargo"]);
        
        $arDataContact = $this->from_post_to_data($_POST);
        $arDataMtbContacto = $this->from_post_to_data($_POST, "detm_");
        
        $arDataMtbContacto["Modify_User"] = $this->_session_user_code;
        $arDataMtbContacto["Modify_Date"] = $this->get_hydra_date();
        $arDataContact["Modify_User"] = $arDataMtbContacto["Modify_User"];
        $arDataContact["Modify_Date"] = $arDataMtbContacto["Modify_Date"];
        $arDataMtbContacto["Enviado_a_Erp"] = "0";
        
        //Creo la descripcion
        $sDescripcion = $oContact->get_first_name()." ".$oContact->get_last_name();
        $sCargo = $oContact->get_cargo_descripcion();
        if(!empty($sCargo)) $sDescripcion .= " - $sCargo";
        $arDataContact["Description"] = $sDescripcion;
        
        $sCodeContact = $arDataContact["Code"];
        $arCondition = array("Code"=>$sCodeContact);
        $oContact->auto_update($arDataContact,$arCondition,array("Code"));
        //$oMtbCliente->set_numeric_fields(array("Credito_Caution","Descuento","Descuento2","Dto_ProntoPago"));
        $oMtbContacto->auto_update($arDataMtbContacto,$arCondition,array("Code"));
        
        $this->go_to_detail($sCodeContact);
    }
    
    private function save_new() 
    { 
        $sCodeAccount = $_POST["det_Code_Account"];
        $oClient = new ModelClients(null,$sCodeAccount);
        //usuario en sesion no lo necesito
        $oContact = new ModelContacts(null, null, $oClient);
        $oContact->set_first_name($_POST["det_First_Name"]);
        $oContact->set_last_name($_POST["det_Last_Name"]);
        $oContact->set_cargo($_POST["det_Cargo"]);
        $oMtbContacto = new ModelMtbContactos($oContact);
        
        //Creo los arrays de datos
        $arDataContact = $this->from_post_to_data($_POST,"det_");
        $arDataMtbContacto = $this->from_post_to_data($_POST, "detm_");
        
        $arDataContact["Create_User"] = $this->_session_user_code;
        $arDataContact["Create_Date"] = $this->get_hydra_date();
        $arDataMtbContacto["Create_User"] = $arDataContact["Create_User"];
        $arDataMtbContacto["Create_Date"] = $arDataContact["Create_Date"];  
         
        $arDataContact["Modify_User"] = $arDataContact["Create_User"];
        $arDataContact["Modify_Date"] = $arDataContact["Create_Date"];         
        $arDataMtbContacto["Modify_User"] = $arDataContact["Create_User"];
        $arDataMtbContacto["Modify_Date"] = $arDataContact["Create_Date"]; 
        
        //Creo la descripcion
        $sDescripcion = $oContact->get_first_name()." ".$oContact->get_last_name();
        $sCargo = $oContact->get_cargo_descripcion();
        if(!empty($sCargo))
            $sDescripcion .= " - $sCargo";
        $arDataContact["Description"] = $sDescripcion;
        
        $iCodeErp = $oContact->get_key_erp_by_account();
        $sCodeContact = "$sCodeAccount-$iCodeErp";
        $oContact->set_keys_and_values(array("Code"=>$sCodeContact));
        $oContact->auto_insert($arDataContact);

        $arDataMtbContacto["Codigo_Erp"] = $iCodeErp;
        $arKeysAndValues = $oContact->get_keys_and_values();
        $oMtbContacto->set_keys_and_values($arKeysAndValues);
        //$oMtbContacto->set_numeric_fields(array("Credito_Caution","Descuento","Descuento2","Dto_ProntoPago"));
        $oMtbContacto->auto_insert($arDataMtbContacto);
        
        $this->go_to_detail($sCodeContact);
    }
    
    private function load_detail_picklists()
    {
        $sSQLAnd = "Type='3'";
        $arOptions = MainModel::get_data_for_picklist("contacts_array", "Code", "Description", $sSQLAnd);
        $oSelCargo = new HelperSelect($arOptions, "det_Cargo","Cargo");
        $this->oGParams->set_in_vars("oSelCargo",$oSelCargo);
        
        //Sirve para ejecutar onready
        $oHlpJavascript = new HelperJavascript();
        $arRequired = array("det_First_Name","det_Last_Name");
        $oHlpJavascript->set_required_fields($arRequired);
        $this->oGParams->set_in_vars("oHlpJavascript", $oHlpJavascript);
    }

    private function is_new()
    {
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
        return $isNew;
    }
    
    private function is_by_account()
    {
        if(!empty($this->_code_account))
            $this->oGParams->set_in_vars("isByAccount", true); return true; 

        $this->oGParams->set_in_vars("isByAccount", false);
        return false;
    }

    public function go_to_detail($sCodeContact)
    {
        $_POST = null; $_GET = null;
        header("Location: $this->_link_url_detail&Code=$sCodeContact");
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
        }
    }	
    
    //NOT IN USE
    private function list_main()
    {
        
    }    
}
