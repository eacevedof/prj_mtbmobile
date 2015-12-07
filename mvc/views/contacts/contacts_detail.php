<?php
if(!isset($oContact)) $oContact = new ModelContacts(null,null);
if(!isset($oMtbContacto)) $oMtbContacto = new ModelMtbContactos($oContact);

$oSubmitButtons = new HelperSubmitButton();
$oSubmitButtons->display_delete(!$isNew);
if($isNew)
{
    //Desde módulos externos
    if($isByAccount)
    {
        $sCodeAccount = $oContact->oClient->get_code();
        $oSelClients->set_value_to_select($sCodeAccount);
        //No error, 1ra Vez
        if(empty($sErrorMessage))
        {
            $oHlpJavascript->show_errormessage($sErrorMessage,20);
        }
    }
}
//Modificando
else
{
    $sCodeAccount = $oContact->oClient->get_code();
    $oSelClients->set_value_to_select($sCodeAccount);
    $oSelCargo->set_value_to_select($oContact->get_cargo());
}

//HTML
echo $oHlpJavascript->get_document_onready();
//Barra del propietario
$oHlpOwnerInfoBar->show();
//Botones foreign del propietario
$oHlpFrgLink->show();
$oHlpActionBar->show();
?>
<form id="frmDetail" name="frmDetail" action="" method="POST" class="ui-body-c ui-corner-all buttons-bar" >
    <div id="divForm" style="padding-left:10px; padding-right:5px;" >
        <div id="divMessage" style="display: none"></div>
<?php
//Botones de formulario

$oSubmitButtons->show_top_div();
//bug($oSubmitButtons); die;
//Código
$oTxtCodigo = new HelperText("det_Code",$oContact->get_code(),"Código");
$oTxtCodigo->readonly();
$oTxtCodigo->set_keycode();
$oTxtCodigo->display(!$isNew);
//Nombre
$oTxtNombre = new HelperText("det_First_Name",$oContact->get_first_name(),"Nombre");
$oTxtNombre->required();

//Apellido
$oTxtApellido = new HelperText("det_Last_Name",$oContact->get_last_name(),"Apellido");
$oTxtApellido->required();
//Apellido 2
$oTxtSegApellido = new HelperText("detm_Last_Name2",$oMtbContacto->get_last_name2(),"Seg. Apellido");
//Telefono
$oTelTelefono1 = new HelperText("det_Tlf1",$oContact->get_tlf1(),"Teléfono");
$oTelTelefono1->set_type("tel");
//Movil
$oTelMovil = new HelperText("det_Tlf2",$oContact->get_tlf2(),"Móvil");
$oTelMovil->set_type("tel");
//Fax
$oTelFax = new HelperText("det_Fax",$oContact->get_fax(),"Fax");
$oTelFax->set_type("tel");
//Email
$oEmail = new HelperText("det_Email",$oContact->get_email(),"Email");
$oEmail->set_type("email");

//Muestro los controles en pantalla
$oTxtCodigo->show();
//Select clientes
$oSelClients->show();
$oTxtNombre->show();
$oTxtApellido->show();
$oTxtSegApellido->show();
$oTelTelefono1->show();
$oTelMovil->show();
$oTelFax->show();
$oEmail->show();

$oSelCargo->show();
$oSubmitButtons->show_bottom_div();
?>       
        <input type="hidden" name="module" id="hidModule" value="contacts" />
        <input type="hidden" name="tab" id="hidTab" value="detail" />
        <input type="hidden" name="action" id="hidAction" value="save" /> 
    </div>    
</form>
