<?php
//$oSite = new Site();
if(!isset($oMtbPropuesta))  $oMtbPropuesta = new ModelMtbPropuestaComercial(null,null);

$oSubmitButtons = new HelperSubmitButton();
$oSubmitButtons->display_delete(false);

//CAMPOS
$oHidTypeofNew = new HelperHidden("Type",$oMtbPropuesta->get_typeof_new());
$oHidCodeAccount = new HelperHidden("Code_Account", $oMtbPropuesta->get_code_cliente());
$oHidCodeCliente = new HelperHidden("detm_Code_Cliente", $oMtbPropuesta->get_code_cliente());

$oTxtDescFamilia = new HelperText("Familia_Description",$oMtbPropuesta->get_descripcion_familia(),"Familia");
$oTxtDescFamilia->required();
$oTxtDescFamilia->readonly();

$oHidCodeFamilia = new HelperHidden("detm_Code_Familia",$oMtbPropuesta->get_code_familia());
$oNumberDto = new HelperText("detm_Dto",$oMtbPropuesta->get_dto(),"Dto");
$oNumberDto->required();
$oNumberDto->set_type("number");

$oHidValorOld = new HelperHidden("detm_Valor_Old",$oMtbPropuesta->get_valor_old());

if($isNew)
{
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
            //bug("by acc 1+");
            //$oHlpJavascript->show_advisor($sErrorMessage);
            $oHlpJavascript->show_errormessage($sErrorMessage);
            
            
        }

    }//fin by account
    //Por defecto desde menu general
    else
    {    }//Fin alta
}
//Modificando
else
{
    //Hay error
    if(!empty($sErrorMessage))
    {
        $oHlpJavascript->show_errormessage($sErrorMessage);
    }

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
$oHidTypeofNew->show();
$oHidCodeAccount->show();
$oHidCodeCliente->show();
$oTxtDescFamilia->show();
$oHidCodeFamilia->show();
$oNumberDto->show();
$oHidValorOld->show();
$oSubmitButtons->show_bottom_div();
?>                
        <input type="hidden" id="hidModule" name="module" value="propuestas" />
        <input type="hidden" id="hidTab" name="tab"  value="detail" />
        <input type="hidden" id="hidAction" name="action" value="save" /> 
    </div>
</form>

