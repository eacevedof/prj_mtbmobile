<?php
//$oSite = new Site();
if(!isset($oMtbPropuesta))  $oMtbPropuesta = new ModelMtbPropuestaComercial(null,null);

$oSubmitButtons = new HelperSubmitButton();
$oSubmitButtons->display(false);

//CAMPOS
$oTxtCodigo = new HelperText("detm_Code",$oMtbPropuesta->get_code(),"Código");
$oTxtCodigo->readonly();
$oTxtCodigo->set_keycode();

$oHidCodeAccount = new HelperHidden("Code_Account", $oMtbPropuesta->get_code_cliente());
$oHidCodeCliente = new HelperHidden("detm_Code_Cliente", $oMtbPropuesta->get_code_cliente());

//Existen 3 tipos "Familia", "Subfamilia" y "Producto"
$sConcepto = $oMtbPropuesta->get_concepto();
$oTxtConcepto = new HelperText("detm_Concepto",$oMtbPropuesta->get_concepto(),"Concepto");
$oTxtConcepto->readonly();

$oSelFamilia->set_value_to_select($oMtbPropuesta->get_code_familia());
$oSelFamilia->readonly();

$oSelSubfamilia->set_value_to_select($oMtbPropuesta->get_code_subfamilia());
$oSelSubfamilia->readonly();

$oSelProducto->set_value_to_select($oMtbPropuesta->get_code_articulo());
$oSelProducto->readonly();

$oNumberDto = new HelperText("detm_Dto",$oMtbPropuesta->get_dto(),"Dto");
$oNumberDto->readonly();

$oTxtValorOld = new HelperText("detm_Valor_Old",$oMtbPropuesta->get_valor_old(),"Dto Anterior");
$oTxtValorOld->readonly();

$oFechaSolicitud = new HelperDate("detm_FechaS",$oMtbPropuesta->get_fechas(),"Fec. Sol");
$oFechaSolicitud->readonly();

$oFechaRespuesta = new HelperDate("detm_FechaS",$oMtbPropuesta->get_fechar(),"Fec. Resp");
$oFechaRespuesta->readonly();

$oTxaDescripcion = new HelperTextArea("detm_Descripcion",$oMtbPropuesta->get_descripcion(),"Descripcion");
$oTxaDescripcion->readonly();

$oSelEstado->readonly();
$oSelEstado->set_value_to_select($oMtbPropuesta->get_estado());

//Las altas se hacen desde el listado de descuentos
//se utilizan otras vistas. Esta alta se podria utilizar en un futuro.
if($isNew)
{
    /*
    //Modulos externos
    if($isByAccount)
    {

        //No error, 1ra Vez
        if(empty($sErrorMessage))
        {
          
        }
        //Error al guardar
        else
        {
            $oHlpJavascript->show_errormessage($sErrorMessage);
        }

    }//fin by account
    //Por defecto desde menu general
    else
    {    }//Fin alta*/
}
//Modificando
else
{
    switch ($sConcepto) 
    {
        case "Familia":
            $oSelSubfamilia->display(false);
            $oSelProducto->display(false);
        break;
        case "Subfamilia":
            $oSelProducto->display(false);
        break;
        default:
        break;
    }    
    
    //Hay error
    if(!empty($sErrorMessage))
        $oHlpJavascript->show_errormessage($sErrorMessage);
    else
        ;
    
    

}

//CÓDIGO HTML
//$oHlpJavascript = new HelperJavascript();
//$oSite = new Site();
if(empty($sSuccessMessage)) 
{
    $sSuccessMessage = $oSite->get_from_session("sSuccesMessage");
    $oSite->delete_from_session("sSuccesMessage");
}
$oHlpJavascript->show_successmessage($sSuccessMessage);
echo $oHlpJavascript->get_document_onready();
//Estas no están disponibles desde el alta del menu general
//Barra del propietario
if(!empty($oOwnerInfoBar))$oOwnerInfoBar->show();
//Botones foreign del propietario
if(!empty($oFrgLink))$oFrgLink->show();
$oActionBar->show();

?>
<form id="frmDetail" name="frmDetail" action="" method="POST" class="ui-body-c ui-corner-all buttons-bar" >
    <div id="divForm" style="padding-left: 10px;  padding-right: 5px;" >
        <div id="divMessage" style="display: none"></div>
    
<?php
$oSubmitButtons->show_top_div();

$oTxtCodigo->show();
$oHidCodeAccount->show();
$oHidCodeCliente->show();
$oTxtConcepto->show();

$oSelFamilia->show();
$oSelSubfamilia->show();
$oSelProducto->show();

$oNumberDto->show();
$oTxtValorOld->show();

$oFechaSolicitud->show();
$oFechaRespuesta->show();

$oTxaDescripcion->show();
$oSelEstado->show();
$oSubmitButtons->show_bottom_div();
?>                
        <input type="hidden" id="hidModule" name="module" value="propuestas" />
        <input type="hidden" id="hidTab" name="tab"  value="detail" />
        <input type="hidden" id="hidAction" name="action" value="save" /> 
    </div>
</form>

