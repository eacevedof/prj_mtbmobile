<?php
//$oSite = new Site();

if(!isset($oActivity))  $oActivity = new ModelActivities(null,null);
if(!isset($oMtbActivity)) $oMtbActivity = new ModelMtbActivities($oActivity);

//bug($oActivity->oClient); die;
$oSubmitButtons = new HelperSubmitButton();
$oSubmitButtons->display_delete(false);

$oTxtCodigo = new HelperText("det_Code",$oActivity->get_code(),"Código");
$oTxtCodigo->readonly();
$oTxtCodigo->set_keycode();
$oTxtCodigo->display(!$isNew);

$oDateFecha = new HelperDate("det_DateP", $oActivity->get_datep(), "Fecha");
$oDateFecha->required();
$oDateFecha->readonly(!$isNew);
$oDateFecha->set_today();

//Resultado
$oTxaResultado = new HelperTextArea("detm_Resultado",$oMtbActivity->get_resultado(),"Resultado");
//Interlocutores
$oTxaInterlocutores = new HelperTextArea("det_Notes",$oActivity->get_notes(),"Interlocutores");
//Interlocutores Next
$oTxaInterlocutoresNext = new HelperTextArea("det_Notas_Centralita",$oActivity->get_notas_centralita(),"Interlocutores");

//bug($oActivity); die;
//Configuración de objetos
if($isNew)
{
    //Modulos externos
    if($isByAccount)
    {
        $sCodeAccount = $oActivity->oClient->get_code();
        $sPropietario = $oActivity->oClient->get_propietario();
        //No error, 1ra Vez
        if(empty($sErrorMessage))
        {
            //Comercial
            $oSelUser->set_value_to_select($sPropietario);
            $oSelUser->required();
            $oSelUser->readonly();

            //Cliente
            $oSelClient->set_value_to_select($sCodeAccount);
            $oSelClient->readonly();
            //Accion no se autoselecciona
            //Horas
            $oSelHoraInicio->set_value_to_select("18");
            $oSelHoraFinal->set_value_to_select("18");
            //Resultado
            $oSelEstado->set_value_to_select("0"); //bug($oSelEstado); //0:Pendiente 
            $oSelEstado->readonly();          
            //Fecha
            //$oHlpFecha
        }
        //Error al guardar
        else
        {
            //bug("by acc 1+");
            //$oHlpJavascript->show_advisor($sErrorMessage);
            $oHlpJavascript->show_errormessage($sErrorMessage);
            
            //Comercial
            $oSelUser->set_value_to_select($oActivity->get_code_user());
            $oSelUser->readonly();

            //Cliente
            $oSelClient->set_value_to_select($oActivity->get_code_account());
            $oSelClient->readonly();

            //Contacto
            $oSelContacto->set_value_to_select($oActivity->get_code_contact());    
            $oSelContacto->readonly();
    
            //Accion
            $oSelAccion->set_value_to_select($oActivity->get_code_newtype());
            //Fecha
            $oDateFecha->set_value($oActivity->get_datep());
            
            $oSelHoraInicio->set_value_to_select($oActivity->get_start_time());
            $oSelHoraFinal->set_value_to_select($oActivity->get_end_time());

            $oSelEstado->set_value_to_select($oActivity->get_code_result()); //bug($oSelEstado); //0:Pendiente 
        }

    }//fin by account
    //Por defecto desde el usuario
    else
    {
        //bug("no es desde el usuario:$isByAccount"); 
        $sCodeUser = $oSite->get_logged_user_code();
        $oSelUser->set_js_onchange("oMtbAjax.load_clientes(this);");
        $oSelClient->set_js_onchange("oMtbAjax.load_contactos(this);");
        //No error, 1ra Vez
        if(empty($sErrorMessage))
        {
            //Comercial
            $oSelUser->set_value_to_select($sCodeUser);
            $oSelUser->required();
            //Cliente
            $oSelClient->set_value_to_select($sCodeAccount);
            $oSelClient->required();
            //Contacto
            //$oSelContacto->required();
            
            //Accion no se autoselecciona
            //Horas
            $oSelHoraInicio->set_value_to_select("18");
            $oSelHoraFinal->set_value_to_select("18");
            //Resultado
            $oSelEstado->set_value_to_select("0"); //bug($oSelEstado); //0:Pendiente 
            $oSelEstado->readonly();          
            //Fecha
            //$oHlpFecha
        }
        //Error al guardar
        else
        {
            //bug("by acc 1+");
            //$oHlpJavascript->show_advisor($sErrorMessage);
            $oHlpJavascript->show_errormessage($sErrorMessage);
            
            //Comercial
            $oSelUser->set_value_to_select($oActivity->get_code_user());
            $oSelUser->readonly();

            //Cliente
            $oSelClient->set_value_to_select($oActivity->get_code_account());
            $oSelClient->readonly();

            //Contacto
            $oSelContacto->set_value_to_select($oActivity->get_code_contact());    
            $oSelContacto->readonly();
    
            //Accion
            $oSelAccion->set_value_to_select($oActivity->get_code_newtype());
            //Fecha
            $oDateFecha->set_value($oActivity->get_datep());
            
            $oSelHoraInicio->set_value_to_select($oActivity->get_start_time());
            $oSelHoraFinal->set_value_to_select($oActivity->get_end_time());

            $oSelEstado->set_value_to_select($oActivity->get_code_result()); //bug($oSelEstado); //0:Pendiente 
        }//Fin error        
    }//Fin alta
}
//Modificando
else
{
    $oDivSeparator = new HelperDivHeader("Replanificación");
    $oDateFechaProxima = new HelperDate("det_Date_Next", $oActivity->get_date_next(),"Fecha Próxima");
    $oSubmitButtons->display_replan();
    
    //Hay error
    if(!empty($sErrorMessage))
    {
        $oHlpJavascript->show_errormessage($sErrorMessage);
    }
    
    //bug("modificando"); die;
    //Accion
    $oSelAccion->set_value_to_select($oActivity->get_code_newtype());
    $oSelAccion->readonly();
    
    $oSelProximaAccion->set_value_to_select($oActivity->get_code_newtype_next());
    
    //Comercial
    $oSelUser->set_value_to_select($oActivity->get_code_user());
    $oSelUser->readonly(); 

    //Cliente
    $oSelClient->set_value_to_select($oActivity->get_code_account());
    $oSelClient->readonly();
        
    //Contacto
    $oSelContacto->set_value_to_select($oActivity->get_code_contact());    
    $oSelContacto->readonly();
    
    $oSelHoraInicio->set_value_to_select($oActivity->get_start_time());
    
    $oSelHoraFinal->set_value_to_select($oActivity->get_end_time());
    
    $oSelEstado->set_value_to_select($oActivity->get_code_result()); //bug($oSelEstado); //0:Pendiente 
    
    //CAMPOS REPLANIFICACION
    $oSelProximaAccion->set_value_to_select($oActivity->get_code_newtype_next());
    //Próxima fecha
    $oDateFechaProxima->set_class("clsRequired");

    //Completada o replanificada
    if($oActivity->get_code_result()!="0")
    {
        //bug($oActivity->get_code_result()); die;
        $oDateFecha->readonly();
        $oSelEstado->readonly();
        $oSelHoraInicio->readonly();
        $oSelHoraFinal->readonly();
        $oSelProximaAccion->readonly();
        $oTxaResultado->readonly();
        $oTxaInterlocutores->readonly();
        
        //Si hay replanificacion
        //bug($oActivity->get_date_next()); die;
        if($oActivity->get_date_next()!="")
        {    
            $oSelProximaAccion->readonly();
            $oDateFechaProxima->readonly();
            $oTxaInterlocutoresNext->readonly();
        }
        //Si no hay replanificación (get_date_next!="")
        else
        {
            $oDivSeparator->display(false);
            $oSelProximaAccion->display(false);
            $oDateFechaProxima->display(false);
            $oTxaInterlocutoresNext->display(false);
        }
        $oSubmitButtons->display(false);
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
//Código
$oTxtCodigo->show();
//Vendedor
$oSelUser->show();

//Select clientes
$oSelClient->show();
//Hidden. Solo existirá si se esta creando desde módulo externo
if(!empty($oHidCodeAccount)) $oHidCodeAccount->show();

//Contacto
$oSelContacto->show();
//Acccion
$oSelAccion->show();
//Fecha
$oDateFecha->show();
//Hora Inicio
$oSelHoraInicio->show();
//HoraFin
$oSelHoraFinal->show();
//Resultado
$oTxaResultado->show();
//Interlocutores
$oTxaInterlocutores->show();

//Estado
$oSelEstado->show();

//Campos extra solo para edición
if(!$isNew)
{    
    $oDivSeparator->show();
    //Proxima Accion
    $oSelProximaAccion->show();
    //Fecha próxima visita
    $oDateFechaProxima->show();
    $oTxaInterlocutoresNext->show();
}//Fin campos extra para edición
$oSubmitButtons->show_bottom_div();
?>                
        <input type="hidden" id="hidModule" name="module" value="activities" />
        <input type="hidden" id="hidTab" name="tab"  value="detail" />
        <input type="hidden" id="hidAction" name="action" value="save" /> 
    </div>
</form>

