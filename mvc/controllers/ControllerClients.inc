<?php
class ControllerClients extends MainController implements InterfaceController  
{
    
    public function __construct()
    {
        parent::__construct();
        $this->_list_page = $_POST["hidNumPage"];
        $this->load_foreign_links();
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=list";
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail";
    }
   
    public function basic_list() 
    {
        $oClient = new ModelClients($this->_session_user_code, null);
        //Datos para paginación. La primera vez list_page=null. Esto lo controla
        //el componente paginate en el modelo
        $oClient->set_list_page($this->_list_page);
        
        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Name","Poblacion","Codigo_Sic","Cp");
        if($this->is_searching($arSearchFields,"oClientSearch"))
        {    
            //bug("is searching");
            $oClient->set_name($_POST["sch_Name"]);
            $oClient->set_mtb_poblacion ($_POST["sch_Poblacion"]);
            $oClient->set_codigo_sic($_POST["sch_Codigo_Sic"]);
            $oClient->set_cp($_POST["sch_Cp"]);
            $this->oSite->set_in_session("oClientSearch", $oClient);
        }
        $arDataList = $oClient->get_mobile_list();
        //bug($arDataList);
        $this->_list_num_pages = $oClient->get_list_num_pages();
        $this->_list_total_regs = $oClient->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oClient->get_list_page();
        $arPagesForSelect = $oClient->get_list_pages();
        $oSelPage = new HelperSelect($arPagesForSelect, "selPage","",$this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this)");
        
        //ActionBar
        $oHlpActionBar = new HelperActionBar("Clientes", "Listado");
        //Dibuja el listado
        $oHlpList = new HelperList($arDataList,"Nombre", array("Code"), array("Poblacion","Cp","Vendedor"), 
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "clients","detail",array("Code"));
        
        //Variables a utilizar en la vista
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        $this->oView->add_file("clients_list.php");
        $this->oView->display();
    }
    
    public function detail()
    {
        //MTB_Codigos_Postales
        if($this->is_saving())
        {
            if($this->is_new())
            {
                Debug::set_message("save new");
                $this->save_new();
            }
            else
            {
                Debug::set_message("save old");
                $this->save_edit();
            }            
        }
        //Leyendo
        else
        {
            $this->load_detail_picklists();
            
            if($this->is_new())
            {
                Debug::set_message("Read new");
                $this->read_new();
            }
            else
            {
                Debug::set_message("Read old");
                $codClient = $_POST["Code"];
                $this->read_edit($codClient);
            }
        }
        $this->oView->add_file("clients_detail.php");
        $this->oView->display();           
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

    private function read_edit($codClient)
    {
        //Objetos
        $oClient = new ModelClients($this->_session_user_code, $codClient);
        $oClient->load_by_code();
        $oMtbCliente = new ModelMtbClientes($oClient);
        $oMtbCliente->load_by_code();
        
        //Owner Bar
        $sPDescription = $codClient." - ".$oClient->get_name();
        $sLnkDetail = $this->_link_url_detail."&Code=$codClient";
        $oHlpOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sPDescription,$sLnkDetail);
        
        //ActionBar
        $sLnkSearch = $this->_link_url_list;
        $oHlpActionBar = new HelperActionBar("Detalle", "", $sLnkSearch,$sLnkDetail);
        
        //FrgLinks
        $oHlpFrgLink = new HelperForeignLinks($this->_arFrgLinks,$codClient);
        
        $this->oGParams->set_in_vars("oClient", $oClient);
        $this->oGParams->set_in_vars("oMtbCliente", $oMtbCliente);
        $this->oGParams->set_in_vars("oHlpOwnerInfoBar", $oHlpOwnerInfoBar);
        $this->oGParams->set_in_vars("oHlpFrgLink", $oHlpFrgLink);
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);
    }

    private function read_new()
    {
        //Objetos vacios
        $oClient = new ModelClients(null, null);
        $oMtbCliente = new ModelMtbClientes(null);
        $oHlpActionBar = new HelperActionBar("Nuevo Cliente", "", $this->_link_url_list,$this->_link_url_detail);
        
        $this->oGParams->set_in_vars("oClient", $oClient);
        $this->oGParams->set_in_vars("oMtbCliente", $oMtbCliente);
        $this->oGParams->set_in_vars("oHlpActionBar", $oHlpActionBar);
    }
    
    private function load_detail_picklists()
    {
        //Picklists
        $arOptions = MainModel::get_data_for_picklist("MTB_Empresas", "Code", "Nombre");
        $oSelEmpresa = new HelperSelect($arOptions, "detm_Empresa","Empresa");
        $this->oGParams->set_in_vars("oSelEmpresa",$oSelEmpresa);
        
        $arOptions = MainModel::get_data_for_picklist("modules_common_countries");
        $oSelPais = new HelperSelect($arOptions, "det_Pais","País","País");
        $this->oGParams->set_in_vars("oSelPais",$oSelPais);

        $arOptions = MainModel::get_data_for_picklist("modules_common_provinces");
        $oSelProvincia = new HelperSelect($arOptions, "det_Provincia","Provincia");
        $this->oGParams->set_in_vars("oSelProvincia",$oSelProvincia);

        //Cp llenarlo con ajax segun provincia
        $codCliente = $_POST["Code"];
        $sSQLAnd = "Code_Provincia = (SELECT Provincia FROM clients WHERE Code='$codCliente')";
        $arOptions = MainModel::get_data_for_picklist("MTB_Codigos_Postales", "Code", "Cp", $sSQLAnd);
        $oSelCp = new HelperSelect($arOptions, "det_Cp","CP");
        $this->oGParams->set_in_vars("oSelCp",$oSelCp);

        $arOptions = MainModel::get_data_for_picklist("MTB_FormaPago","Code","Descripcion");
        $oSelFormaPago = new HelperSelect($arOptions, "detm_Forma_Pago","Forma Pago");
        $this->oGParams->set_in_vars("oSelFormaPago",$oSelFormaPago);
        
        $arOptions = MainModel::get_data_for_picklist("MTB_Array","Code","Description","Type='estado clien'");
        $oSelEstado = new HelperSelect($arOptions, "detm_Estado","Estado");
        $this->oGParams->set_in_vars("oSelEstado",$oSelEstado);        
        
        //Users
        $sUserCondition = "Code NOT LIKE 't%' 
                           AND Code NOT LIKE 'x%' 
                           AND Code IN (SELECT Code FROM get_code_vendedores('$this->_session_user_code'))
                           OR Code='t13' ";
        $arOptions = MainModel::get_data_for_picklist("core_users","Code","Description",$sUserCondition);
        $oSelVendedor = new HelperSelect($arOptions, "det_Propietario","Vendedor");
        $oSelVendedor->required();
        //bug($oSelVendedor);die;
        $this->oGParams->set_in_vars("oSelVendedor",$oSelVendedor);

        //Campo unicamente informativo
        $sUserCondition = "Code IN (SELECT DISTINCT Code_Manager FROM core_users 
                              WHERE Code_Manager IS NOT NULL )";
        $arOptions = MainModel::get_data_for_picklist("core_users","Code","Description",$sUserCondition);
        $oSelManager = new HelperSelect($arOptions, "Code_Manager","Manager");
        $oSelManager->readonly();
        $this->oGParams->set_in_vars("oSelManager",$oSelManager);            
     
        $arOptions = array("0"=>"NO","1"=>"SI");
        $oRadInformacion= new HelperRadio($arOptions, "detm_Desea_Informacion","Desea Información");
        $this->oGParams->set_in_vars("oRadInformacion",$oRadInformacion);
        
        $oHlpJavascript = new HelperJavascript();
        $oHlpJavascript->set_required_fields(array("det_Name","det_Propietario"));
        $this->oGParams->set_in_vars("oHlpJavascript",$oHlpJavascript);
    }
    
    public function save_edit()
    {
        $oClient = new ModelClients();
        $oMtbCliente = new ModelMtbClientes();
        
        $arDataClient = $this->from_post_to_data($_POST);
        $arDataMtbCliente = $this->from_post_to_data($_POST, "detm_", array("Fecha_Alta"));
        
        $arDataMtbCliente["Modify_User"] = $this->_session_user_code;
        $arDataMtbCliente["Modify_Date"] = $this->get_hydra_date();
        $arDataClient["Modify_User"] = $arDataMtbCliente["Modify_User"];
        $arDataClient["Modify_Date"] = $arDataMtbCliente["Modify_Date"];
        
        $arDataMtbCliente["Ultima_Operacion"] = "M";//Agregar
        $arDataMtbCliente["Validado"] = "0";
        $arDataMtbCliente["Enviado_a_Erp"] = "0";
        
        $codClient = $arDataClient["Code"];
        $arCondition = array("Code"=>$codClient);
        $oClient->auto_update($arDataClient,$arCondition,array("Code"));
        $oMtbCliente->set_numeric_fields(array("Credito_Caution","Descuento","Descuento2","Dto_ProntoPago"));
        $oMtbCliente->auto_update($arDataMtbCliente,$arCondition,array("Code"));
        $oMtbCliente->set_code($codClient);
        //Actualiza validado=0 ultima_operacion=M y Enviado_Erp=0
        //$oMtbCliente->update_after_edit();
        $this->go_to_detail($codClient);
    }
    
    public function save_new() 
    { 
        $oClient = new ModelClients();
        $oMtbCliente = new ModelMtbClientes();
        
        $arDataClient = $this->from_post_to_data($_POST,"det_");
        $arDataMtbCliente = $this->from_post_to_data($_POST, "detm_", array("Fecha_Alta"));
        
        $arDataClient["Create_User"] = $this->_session_user_code;
        $arDataClient["Create_Date"] = $this->get_hydra_date();
        $arDataMtbCliente["Create_User"] = $arDataClient["Create_User"];
        $arDataMtbCliente["Create_Date"] = $arDataClient["Create_Date"];  
         
        $arDataClient["Modify_User"] = $arDataClient["Create_User"];
        $arDataClient["Modify_Date"] = $arDataClient["Create_Date"];         
        $arDataMtbCliente["Modify_User"] = $arDataClient["Create_User"];
        $arDataMtbCliente["Modify_Date"] = $arDataClient["Create_Date"]; 
        //Campos de control
        $arDataMtbCliente["Ultima_Operacion"] = "A";//Agregar
        $arDataMtbCliente["Validado"] = "0";
        $arDataMtbCliente["Enviado_a_Erp"] = "0";
        
        $oClient->set_autokeys_on();
        $oClient->set_primarykeys_names(array("Code"));
        $oClient->auto_insert($arDataClient);
        
        $arKeysAndValues = $oClient->get_keys_and_values();
        $oMtbCliente->set_keys_and_values($arKeysAndValues);
        $oMtbCliente->set_numeric_fields(array("Credito_Caution","Descuento","Descuento2","Dto_ProntoPago"));
        $oMtbCliente->auto_insert($arDataMtbCliente);
        //Actualiza validado=0 y ultima_operacion=a
        $oMtbCliente->update_after_new();
        
        $this->go_to_detail($arKeysAndValues["Code"]);
    }

    private function go_to_detail($codClient)
    {
        $_POST = null; $_GET = null;
        header("Location: $this->_link_url_detail&Code=$codClient");
        exit();
    }
    
    public function go_to_list()
    {
        $_POST = null; $_GET = null;
        header("Location: $this->_link_url_list");
        exit();
    }

    public function delete()
    {}		
    
}
