<?php
$oSubmitButtons = new HelperSubmitButton();
$oSubmitButtons->display_delete(false);
//bug($oClient); bug($oMtbCliente); 
if(!isset($oClient))
  $oClient = new ModelClients(null,null);
  
// Code, Direccion, Pais, Tlf1, Provincia, Tlf2, Cp, Fax, Correo_E, Codigo_Sic, Propietario, Name
if(!isset($oMtbCliente))
   $oMtbCliente = new ModelMtbClientes(null,null);

if($isNew)
{
    $oSelVendedor->set_value_to_select($oSite->get_logged_user_code());
    $oSelManager->set_value_to_select($oSite->get_logged_user_code_manager());
}
else
{
    $oSelVendedor->set_value_to_select($oClient->oPropietario->get_code());
    $oSelManager->set_value_to_select($oClient->oPropietario->get_code_manager());    
    //Barra del propietario
    $oHlpOwnerInfoBar->show();
    //Botones foreign del propietario
    $oHlpFrgLink->show();
}

//HTML
//$oHlpJavascript->show_divmessage("hola",20);
echo $oHlpJavascript->get_document_onready();
$oHlpActionBar->show();
?>

<form id="frmDetail" name="frmDetail" action="" method="POST" >
    <div id="divForm" style="padding-left: 8px;  padding-right: 5px;" >
        <div id="divMessage" style="display: none"></div>

<?php
$oTxtCodigo = new HelperText("det_Code",$oClient->get_code(),"Código");
$oTxtCodigo->readonly();
$oTxtCodigo->set_keycode();
$oTxtCodigo->display(!$isNew);

$oTxtNombre = new HelperText("det_Name", $oClient->get_name(),"Nombre");
$oTxtNombre->required();
$oTxtDireccion = new HelperText("det_Direccion", $oClient->get_direccion(),"Dirección");

$oTelTelefono1 = new HelperText("det_Tlf1", $oClient->get_tlf1(),"Teléfono");
$oTelTelefono1->set_type("tel");
$oTelMovil = new HelperText("det_Tlf2", $oClient->get_tlf2(),"Móvil");
$oTelMovil->set_type("tel");
$oTelFax = new HelperText("det_Fax", $oClient->get_fax(),"Fax");
$oTelFax->set_type("tel");
$oEmail = new HelperText("det_Correo_E", $oClient->get_correo_e(),"Email");
$oEmail->set_type("tel");

$oTxtCifNif = new HelperText("det_Codigo_Sic", $oClient->get_codigo_sic(),"Cif/Nif");
$oTxtCifNif->set_js_onblur("is_cif_nif_valido(this);");
$oTxtNumCalle = new HelperText("detm_Numero_Calle", $oMtbCliente->get_numero_calle(),"Num. Calle");
$oTxtPoblacion = new HelperText("detm_Poblacion", $oMtbCliente->get_poblacion(),"Población");
$oTxtIban = new HelperText("detm_Iban", $oMtbCliente->get_iban(),"IBAN");
$oTxtCbango = new HelperText("detm_Cod_Banco", $oMtbCliente->get_cod_banco(),"C. Banco");
$oTxtDc = new HelperText("detm_DC", $oMtbCliente->get_dc(),"DC");
$oTxtNumCuenta = new HelperText("detm_Num_Cuenta", $oMtbCliente->get_num_cuenta(),"N. Cuenta");
$oTxtSucursal = new HelperText("detm_Sucursal", $oMtbCliente->get_sucursal(),"Sucursal");
$oTxtLimCredito = new HelperText("detm_Credito_Caution", $oMtbCliente->get_credito_caution(),"Lim. Credito");
$oTxtDto1 = new HelperText("detm_Descuento", $oMtbCliente->get_descuento(),"Dto. 1");
$oTxtDto2 = new HelperText("detm_Descuento2", $oMtbCliente->get_descuento2(),"Dto. 2");
$oTxtDtoPP = new HelperText("detm_Dto_ProntoPago", $oMtbCliente->get_dto_prontopago(),"Dto. P. Pago");
$oTxaObservaciones = new HelperTextArea("detm_Observaciones",$oMtbCliente->get_observaciones(),"Observ.");
$oTxaObservaciones->set_maxlength(255);

$oDateFechaAlta = new HelperDate("detm_Fecha_Alta", $oMtbCliente->get_fecha_alta(), "Fecha Alta");
$oDateFechaAlta->readonly(!$isNew);
if($isNew)$oDateFechaAlta->set_today();

$oSelEmpresa->set_value_to_select($oMtbCliente->get_empresa());
$oSelPais->set_value_to_select($oClient->get_pais());

$oRadInformacion->set_value_to_check($oMtbCliente->get_desea_informacion());

$oSelProvincia->set_value_to_select($oClient->get_provincia());
$oSelProvincia->set_js_onchange("oMtbAjax.load_codigospostales(this);");
//bug($oSelProvincia);
$oSelCp->set_value_to_select($oClient->get_cp());

$oSelFormaPago->set_value_to_select($oMtbCliente->get_forma_pago());
$oSelEstado->set_value_to_select($oMtbCliente->get_estado());

$oSelVendedor->readonly(!$isNew);
$oSelVendedor->set_js_onchange("oMtbAjax.load_manager(this);");

$oSelManager->readonly();

//MOSTRAR CONTROLES EN PANTALLA
$oSubmitButtons->show_top_div();
$oTxtCodigo->show();
$oTxtNombre->show();
$oTxtDireccion->show();
$oTelTelefono1->show();
$oTelMovil->show();
$oTelFax->show();
$oEmail->show();
$oTxtCifNif->show();
$oTxtNumCalle->show();
$oTxtPoblacion->show();
$oTxtIban->show();
$oTxtCbango->show();
$oTxtDc->show();
$oTxtNumCuenta->show();
$oTxtSucursal->show();
$oTxaObservaciones->show();
$oRadInformacion->show();
$oDateFechaAlta->show();

$oSelEmpresa->show();

$oSelPais->show();
$oSelProvincia->show();

$oSelCp->show();

$oSelFormaPago->show();
$oSelEstado->show();
$oSelVendedor->show();
$oSelManager->show();

$oSubmitButtons->show_bottom_div();
?>  
        
        <input type="hidden" name="module" id="hidModule" value="clients" />
        <input type="hidden" name="tab" id="hidTab" value="detail" />
        <input type="hidden" name="action" id="hidAction" value="save" />                
    </div>
</form>