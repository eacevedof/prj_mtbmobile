<?php
class ControllerProporsals extends MainController implements InterfaceController  
{
    //private $_code_user;
    private $_code_account;
    private $_link_url_list_frg = "";
    
    private $_oMtbPropuesta;

    public function __construct()
    {
        parent::__construct();
        $this->_oMtbPropuesta = new ModelMtbPropuestaComercial();
        $this->_list_page = $_POST["hidNumPage"];
        $this->_code_account = $_POST["Code_Account"];
        //bug($_POST); die;
        $this->load_foreign_links();
        $this->_link_url_detail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."propuestas&tab=detail";
        $this->_link_url_list = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."propuestas&tab=list";
        $this->_link_url_list_frg = $this->_link_url_list ."&Code_Account=$this->_code_account";
    }
    
    public function basic_list() 
    {
        //Se desea listar contactos de una cuenta
        if(!empty($_POST["Code_Account"]))
        {
            $this->list_by_account();
        }
        else
        {
            //Por defecto se mostrara las del usuario
            //$this->list_by_user();
        }
    }

    
    private function list_by_account()
    {
        //bug("list by account"); die; ok
        $sCodeAccount = $this->_code_account;
        //La cuenta pedida
        $oClient = new ModelClients($this->_session_user_code, $sCodeAccount);
        $oClient->load_by_code();
        
        $oMtbPropuesta = new ModelMtbPropuestaComercial($this->_session_user_code);
        $oMtbPropuesta->set_code_cliente($sCodeAccount);
        $oMtbPropuesta->set_list_page($this->_list_page);

        //Comprobación de busqueda. Por campos
        $arSearchFields = array("Code_Articulo","Code_Familia","Code_Subfamilia","Concepto");
        //Si detecta que no se está buscando lo elimina de la sesion
        if($this->is_searching($arSearchFields,"oSchClientProporsal"))
        {   
            $oMtbPropuesta->set_concepto($_POST["sch_Concepto"]);
            $oMtbPropuesta->set_code_articulo($_POST["sch_Code_Articulo"]);
            $oMtbPropuesta->set_code_familia($_POST["sch_Code_Familia"]);
            $oMtbPropuesta->set_code_subfamilia($_POST["sch_Code_Subfamilia"]);
            
            $this->oSite->set_in_session("oSchClientProporsal", $oMtbPropuesta);
        }

        //Datos para el listado
        $arDataList = $oMtbPropuesta->get_mobile_list_by_account();
        //bug($arDataList); die;
        $this->_list_num_pages = $oMtbPropuesta->get_list_num_pages();
        $this->_list_total_regs = $oMtbPropuesta->get_total_regs();
        //Esto es necesario pq el modelo corrige la página si está en fuera de rango
        $this->_list_page = $oMtbPropuesta->get_list_page();
        $arPages = $oMtbPropuesta->get_list_pages();

        //Select con páginas
        $oSelPage = new HelperSelect($arPages, "selPage","", $this->_list_page);
        $oSelPage->set_js_onchange("sel_page_change(this);");
        
        //Owner Bar
        $sOwnDescription = $sCodeAccount." - ".$oClient->get_name();
        $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sOwnDescription, $sPLnkDetail);
        
        //FrgLinks
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("PROPS.");

        //ActionBar
        //$sLnkSearch = $this->_link_url_list_frg;
        $oActionBar = new HelperActionBar(" ");
       
        $oHlpList = new HelperList($arDataList,"Concepto",array("Code"), 
                array("prod_subf_fam","Dto","Valor_Old","Estado"), 
                $this->_list_page, $this->_list_num_pages, $this->_list_total_regs,
                "propuestas","detail",array("Code"));
        $oHlpList->use_tiptext_in_p();
        $oHlpList->use_tiptext_in_h3();
        
        //Variables a utilizar en la vista 
        $this->oGParams->set_in_vars("sCodeAccount", $sCodeAccount);
        $this->oGParams->set_in_vars("oSelPage", $oSelPage);
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oHlpList", $oHlpList);
        
        $this->oView->add_file("clients_proporsals.php");
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
        $this->oView->display();         
    }
    
    private function read_edit()
    {
        if(!$this->_is_error)
        {
            $sCodePropuesta = $_POST["Code"];
            $oMtbPropuesta = new ModelMtbPropuestaComercial($this->_session_user_code,$sCodePropuesta);
            $oMtbPropuesta->load_by_code();
        }
        else
        {
            $sCodePropuesta = $this->_oMtbPropuesta->get_code();
            $oMtbPropuesta = $this->_oMtbPropuesta;
        }
        
        //Owner Info Bar
        $sCodeAccount = $oMtbPropuesta->get_code_cliente();
        $oClient = new ModelClients(null,$sCodeAccount);
        $oClient->load_by_code();
        $sOwnDescription = $sCodeAccount." - ".$oClient->get_name();
        $sOwnDetailLink = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
        $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sOwnDescription,$sOwnDetailLink);
        //si no hay error no hace falta asignar este objeto pq ->oClient se crea con load_by_code
        if($this->_is_error) $oMtbPropuesta->oClient = $oClient;
       
        //Links by account
        $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
        $oFrgLink->set_active_description("PROPS.");
        $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);
        
        //ActionBar
        $sDetailLink = $this->_link_url_detail."&Code=$sCodePropuesta"; 
        $oActionBar = new HelperActionBar("Detalle Propuesta","","",$sDetailLink); //fix in contact
        $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);
        
        //PICKLISTS 
        $sCodeFamilia = $oMtbPropuesta->get_code_familia();
        $sCodeSubamilia = $oMtbPropuesta->get_code_subfamilia();
        //$sCodeArticulo = $oMtbPropuesta->get_code_articulo();
        
        $sSQLAnd = "Code_Family='$sCodeFamilia'";
        $arPicklist = MainModel::get_data_for_picklist("MTB_Products_SubFamily",null,null,$sSQLAnd);
        $oSelSubfamilia = new HelperSelect($arPicklist,"detm_Code_Subfamilia","Subfamilia");
        $this->oGParams->set_in_vars("oSelSubfamilia",$oSelSubfamilia);
        
        $sSQLAnd = "Code_Familia='$sCodeFamilia' AND Code_SubFamilia='$sCodeSubamilia'";
        $arPicklist = MainModel::get_data_for_picklist("MTB_Productos","Code_Producto","Descripcion",$sSQLAnd);
        $oSelProducto = new HelperSelect($arPicklist,"detm_Code_Articulo","Producto");
        $this->oGParams->set_in_vars("oSelProducto",$oSelProducto);
               
        //bug($oMtbPropuesta); die;
        $this->oGParams->set_in_vars("oMtbPropuesta", $oMtbPropuesta);
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oView->add_file("proporsals_detail.php");
    }

    /**
     * Si ocurre un error al intentar de dar de alta se pasa los objetos
     * creados desde POST para volverlos a pintar en el detalle
     */
    private function read_new()
    {
        $oMtbPropuesta = new ModelMtbPropuestaComercial();
        //No es lo mismo detm_Code_Account que Code_Account
        if($this->is_by_account())
        {
            //Cliente dueño de la propuesta
            $oClient = new ModelClients(null,$this->_code_account);
            $oClient->load_by_code();
            $sCodeAccount = $oClient->get_code();
            //Owner Bar
            $sOwnDescription = $sCodeAccount." - ".$oClient->get_name();
            $sPLnkDetail = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."clients&tab=detail&Code=$sCodeAccount";
            $oOwnerInfoBar = new HelperOwnerInfoBar("Cliente", $sOwnDescription, $sPLnkDetail);
            $this->oGParams->set_in_vars("oOwnerInfoBar", $oOwnerInfoBar);

            //FrgLinks
            $oFrgLink = new HelperForeignLinks($this->_arFrgLinks,$sCodeAccount);
            $oFrgLink->set_active_description("PROPS.");
            $this->oGParams->set_in_vars("oFrgLink", $oFrgLink);

            //ActionBar
            //$sLnkSearch = $this->_link_url_list_frg;
            //Code_Account=BGNKP4XQ4KJL&Code_Familia=45&Familia_Description=ABONOS+SIN+CODIFICACION&DtoFamilia=0.00&DtoSubfamilia=NO&Is_Neto=NO&Type=familia            
            //$sLnkRefresh = $this->_link_url_detail."&Code_Account=$sCodeAccount";
            $oActionBar = new HelperActionBar("Nueva Propuesta","","",$sLnkRefresh); 
            
            if($this->is_new_by_family())
            {
                //$sCodeFamilia = $_POST["Code_Familia"];
                $oMtbPropuesta->set_code_cliente($sCodeAccount);
                //$oMtbPropuesta->set_code_agent($this->_session_user_code);
                $oMtbPropuesta->set_code_familia($_POST["Code_Familia"]);
                $oMtbPropuesta->set_descripcion_familia($_POST["Familia_Description"]);
                $oMtbPropuesta->set_dto($_POST["DtoFamilia"]);
                $oMtbPropuesta->set_typeof_new("familia");
                $oMtbPropuesta->set_valor_old($_POST["DtoFamilia"]);
                
                //if($this->_is_error) $oMtbPropuesta = $this->_oMtbPropuesta;
                $this->oView->add_file("proporsals_newbyfamily.php");
            }
            elseif($this->is_new_by_subfamily())
            {
                //bug($_POST); die;
                $oMtbPropuesta->set_code_cliente($sCodeAccount);
                $oMtbPropuesta->set_code_familia($_POST["Code_Familia"]);
                $oMtbPropuesta->set_descripcion_familia($_POST["Familia_Description"]);
                $oMtbPropuesta->set_code_subfamilia($_POST["Code_SubFamilia"]);
                $oMtbPropuesta->set_descripcion_subfamilia($_POST["Subfamilia_Description"]);
                $oMtbPropuesta->set_dto($_POST["Dto1"]);
                $oMtbPropuesta->set_typeof_new("subfamilia");
                $oMtbPropuesta->set_valor_old($_POST["Dto1"]);
                //if($this->_is_error) $oMtbPropuesta = $this->_oMtbPropuesta;
                $this->oView->add_file("proporsals_newbysubfamily.php");
            }
            elseif($this->is_new_by_neto())
            {
                //bug($_POST); die;
                $oMtbPropuesta->set_code_cliente($sCodeAccount);
                $oMtbPropuesta->set_code_familia($_POST["Code_Familia"]);
                $oMtbPropuesta->set_descripcion_familia($_POST["Familia_Description"]);
                $oMtbPropuesta->set_code_subfamilia($_POST["Code_SubFamilia"]);
                $oMtbPropuesta->set_descripcion_subfamilia($_POST["Subfamilia_Description"]);
                $oMtbPropuesta->set_code_articulo($_POST["Code_Articulo"]);
                $oMtbPropuesta->set_descripcion_producto($_POST["Producto_Description"]);
                $oMtbPropuesta->set_dto($_POST["Precio"]);
                $oMtbPropuesta->set_typeof_new("neto");
                $oMtbPropuesta->set_valor_old($_POST["Precio"]);
                
                //if($this->_is_error) $oMtbPropuesta = $this->_oMtbPropuesta;
                $this->oView->add_file("proporsals_newbyneto.php");                
            }
            else
            {
                //if($this->_is_error) $oMtbPropuesta = $this->_oMtbPropuesta;
                $this->oView->add_file("proporsals_detail.php");
            }            
            //Si hay error se recupera desde las variables de clase
            if($this->_is_error)$oMtbPropuesta = $this->_oMtbPropuesta;
                 
            //Hidden que fuerza el crud desde foreign
            $oHidCodeAccount = new HelperHidden("Code_Account",$sCodeAccount);
            $this->oGParams->set_in_vars("oHidCodeAccount", $oHidCodeAccount);
        }
        //Alta desde otros módulos que no sean cuenta
        else 
        {    }
        $this->oGParams->set_in_vars("oActionBar", $oActionBar);
        $this->oGParams->set_in_vars("oMtbPropuesta",$oMtbPropuesta);
    }

    private function save_edit()
    {
        $oMtbPropuesta = new ModelMtbPropuestaComercial();
        $oMtbActivity = new ModelMtbActivities($oMtbPropuesta);
          
        //Creo los arrays de datos
        //bug($_POST); die;
        $arDataMtbPropuesta = $this->from_post_to_data($_POST,"detm_",array("DateP","Date_Next"));
        $arDataMtbActivity = $this->from_post_to_data($_POST,"detm_");

        $arDataMtbActivity["Modify_User"] = $this->_session_user_code;
        $arDataMtbActivity["Modify_Date"] = $this->get_hydra_date();
        $arDataMtbPropuesta["Modify_User"] = $arDataMtbActivity["Modify_User"];
        $arDataMtbPropuesta["Modify_Date"] = $arDataMtbActivity["Modify_Date"];
        
        //bug($arDataMtbPropuesta); bug($arDataMtbActivity); die;
        //Creo la descripcion
        $sCodeActivity = $arDataMtbPropuesta["Code"];
        $arCondition = array("Code"=>$sCodeActivity);
        $oMtbPropuesta->auto_update($arDataMtbPropuesta,$arCondition,array("Code"));

        $oMtbActivity->auto_update($arDataMtbActivity,$arCondition,array("Code"));
        $this->go_to_detail($sCodeActivity);
    }
 
    private function build_description($sConcepto,$sCodeComercial,
            $sNombreComercial,$sFSNDesc,$fDtoOld, $fDto, 
            $sCodeCliente, $sNombreCliente, $sDtoType="Dto")
    {
        $sDescripcion = "Propuesta de \"$sConcepto\". ";
        $sDescripcion .= "EL Comercial $sCodeComercial-$sNombreComercial ";
        $sDescripcion .= "Solicita cambio en $sConcepto: \"$sFSNDesc\" ";
        $sDescripcion .= "del $sDtoType $fDtoOld a $fDto para el cliente: \"$sCodeCliente-$sNombreCliente\""; 
        return $sDescripcion;
    }
    
    private function save_new() 
    { 
        $sTodayDate = date("Ymd");
        
        $oCliente = new ModelClients(null,$_POST["detm_Code_Cliente"]);
        $oCliente->load_by_code();
        
        $sCodeCliente = $oCliente->get_code();
        $sNombreCliente = $oCliente->get_name();
        $sCodeComercial = $this->oSite->get_logged_user_code();
        $sNombreComercial = $this->oSite->get_logged_user_name();
        $sFamiliaDescription = $_POST["Familia_Description"];
        $sSubfamiliaDescription = $_POST["Subfamilia_Description"];
        $sProductoDescription = $_POST["Producto_Description"];
        $fDto = $_POST["detm_Dto"];
        $fDtoOld = $_POST["detm_Valor_Old"];
        
        //Objeto para realizar autoinsert
        $oMtbPropuesta = new ModelMtbPropuestaComercial();
        //Creo los arrays de datos
        $arDataMtbPropuesta = $this->from_post_to_data($_POST,"detm_");
        
        $arDataMtbPropuesta["Code_Agent"] = $sCodeComercial;
        $arDataMtbPropuesta["Is_Neto"] = "0";
        $arDataMtbPropuesta["Estado"] = "P";
        $arDataMtbPropuesta["FechaS"] = $sTodayDate;

        $arDataMtbPropuesta["Code_Agent"] = $sCodeComercial;
        $arDataMtbPropuesta["Create_User"] = $sCodeComercial;
        $arDataMtbPropuesta["Create_Date"] = $this->get_hydra_date();
        $arDataMtbPropuesta["Modify_User"] = $arDataMtbPropuesta["Create_User"];
        $arDataMtbPropuesta["Modify_Date"] = $arDataMtbPropuesta["Create_Date"]; 
        
        if($this->is_new_by_family())
        {
            $sConcepto = "Familia";
            $sDescripcion = $this->build_description($sConcepto, $sCodeComercial, 
                    $sNombreComercial, $sFamiliaDescription, $fDtoOld, $fDto, 
                    $sCodeCliente, $sNombreCliente);
        }
        elseif($this->is_new_by_subfamily())
        {
            $sConcepto = "Subfamilia";
            $sDescripcion = $this->build_description($sConcepto, $sCodeComercial, 
                    $sNombreComercial, $sSubfamiliaDescription, $fDtoOld, $fDto, 
                    $sCodeCliente, $sNombreCliente);
        }
        elseif($this->is_new_by_neto())
        {
            $sConcepto = "Producto";
            $sProductoDescription = $_POST["detm_Code_Articulo"]." - $sProductoDescription";
            $sDescripcion = $this->build_description($sConcepto, $sCodeComercial, 
                    $sNombreComercial, $sProductoDescription, $fDtoOld, $fDto, 
                    $sCodeCliente, $sNombreCliente,"producto");
            $arDataMtbPropuesta["Is_Neto"]="1";
        }        
        
        $arDataMtbPropuesta["Descripcion"] = $sDescripcion;
        $arDataMtbPropuesta["Concepto"] = $sConcepto;
        
        //bug($arDataMtbPropuesta); die;
        $iNewCode = $oMtbPropuesta->get_new_autonum_key();
        $oMtbPropuesta->set_numeric_fields(array("Dto","Valor_Old"));
        $oMtbPropuesta->set_keys_and_values(array("Code"=>$iNewCode));
        $oMtbPropuesta->auto_insert($arDataMtbPropuesta);
        
        $this->go_to_list_by_account($sCodeAccount);
    }
    
    private function is_error_for_new()
    {
        $fDto = trim($_POST["detm_Dto"]);
        $fDto = str_replace(",", ".", $fDto);
        $_POST["detm_Dto"] = $fDto;
        if(!is_numeric($fDto))
        {
            $this->_is_error = true;
            $this->_error_message = "Valor de descuento no válido";
        } 
        /*
        if($this->is_new_by_family())
        {
              
        }
        elseif($this->is_new_by_subfamily())
        {
            
        }
        elseif($this->is_new_by_neto())
        {
            
        }*/
         
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
        $arMustEmpty = array();
        $arMustEmpty[] = $_POST["detm_Date_Next"];
        $arMustEmpty[] = $_POST["detm_Code_Newtype_Next"];
        $arMustEmpty[] = $_POST["detm_Notas_Centralita"];
        
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
      
    /**
     * En caso de error hay que reconstruir los objetos de tipo modelo
     * que intervienen en el formulario desde el array post para volverlos 
     * a mostrar en el formulario
     */
    private function build_objects_from_post()
    {
        $oMtbPropuesta = new ModelMtbPropuestaComercial();
        $oMtbPropuesta->set_code($_POST["detm_Code"]);
        $oMtbPropuesta->set_code_agent($_POST["detm_Code_Agent"]);
        $oMtbPropuesta->set_code_articulo($_POST["detm_Code_Articulo"]);
        $oMtbPropuesta->set_code_cliente($_POST["detm_Code_Cliente"]);
        $oMtbPropuesta->set_code_familia($_POST["detm_Code_Familia"]);
        $oMtbPropuesta->set_code_subfamilia($_POST["detm_Code_SubFamilia"]);
        $oMtbPropuesta->set_concepto($_POST["detm_Concepto"]);
        $oMtbPropuesta->set_dto($_POST["detm_Dto"]);
        $oMtbPropuesta->set_estado($_POST["detm_Estado"]);
        $oMtbPropuesta->set_fechar($_POST["detm_FechaR"]);
        $oMtbPropuesta->set_fechas($_POST["detm_FechaS"]);
        $oMtbPropuesta->set_is_neto($_POST["detm_Is_Neto"]);
        $oMtbPropuesta->set_valor_old($_POST["detm_Valor_Old"]);
        $oMtbPropuesta->set_descripcion($_POST["detm_Descripcion"]);
        $oMtbPropuesta->set_enviado_a_erp($_POST["detm_Enviado_a_Erp"]);

        $oMtbPropuesta->set_id($_POST["detm_Id"]);
        $oMtbPropuesta->set_create_date($_POST["detm_Create_Date"]);
        $oMtbPropuesta->set_create_user($_POST["detm_Create_User"]);
        $oMtbPropuesta->set_modify_date($_POST["detm_Modify_Date"]);
        $oMtbPropuesta->set_modify_user($_POST["detm_Modify_User"]);
        $oMtbPropuesta->set_delete_date($_POST["detm_Delete_Date"]);
        $oMtbPropuesta->set_delete_user($_POST["detm_Delete_User"]);
        
        //Campos extras 
        $oMtbPropuesta->set_descripcion_familia($_POST["Familia_Description"]);
        $oMtbPropuesta->set_descripcion_subfamilia($_POST["Subfamilia_Description"]);
        $oMtbPropuesta->set_descripcion_producto($_POST["Producto_Description"]);
        $oMtbPropuesta->set_typeof_new($_POST["Type"]);
        
        $this->_oMtbPropuesta = $oMtbPropuesta;
    }
    
    private function load_detail_picklists()
    {
        //Familias
        $sSQLAnd = "Type='estado prop'";
        $arOptions = MainModel::get_data_for_picklist("MTB_Array","Code","Description",$sSQLAnd);
        $oSelEstado = new HelperSelect($arOptions, "detm_Estado","Estado");
        $this->oGParams->set_in_vars("oSelEstado",$oSelEstado);
        
        //Familias
        $arOptions = MainModel::get_data_for_picklist("MTB_Products_Family","Code","Description");
        $oSelFamilia = new HelperSelect($arOptions, "detm_Code_Familia","Familia");
        $this->oGParams->set_in_vars("oSelFamilia",$oSelFamilia);
        
        //Sirve para ejecutar onready
        $oHlpJavascript = new HelperJavascript();
        $arRequired = array("detm_Code_Familia","detm_Dto","detm_Code_SubFamilia");
        $oHlpJavascript->set_required_fields($arRequired);
        $this->oGParams->set_in_vars("oHlpJavascript", $oHlpJavascript);
    }

    private function is_new()
    {
        //bug($_POST); die;
        //Type indica que se agregara una propuesta dese descuentos
        if(empty($_POST["Code"])&&empty($_POST["detm_Code"])||!empty($_POST["Type"]))
        {
            //bug($_POST["Code"]); bug($_POST["detm_Code"]);
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
    
    private function is_new_by_family()
    {
        return $_POST["Type"]=="familia";
    }
    
    private function is_new_by_subfamily()
    {
        return $_POST["Type"]=="subfamilia";
    }
    
    private function is_new_by_neto()
    {
        return $_POST["Type"]=="neto";
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
        $sCodeContact = $_POST["detm_Code"];
        $sCodeAccount = $_POST["detm_Code_Account"];
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
