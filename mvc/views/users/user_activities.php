<?php
//$oSelPage = new HelperSelect();
$oActivitySearch = $oSite->get_from_session("oSchUserActivity");
if(empty($oActivitySearch))$oActivitySearch = new ModelActivities();

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
$oActionBar->show();
?>
<form id="frmSearch" name="frmSearch" action="" method="post">

    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <a href="index.php?module=activities&tab=detail" data-inline="true" data-theme="b" data-role="button" class="btn-menu ui-btn ui-btn-inline ui-btn-corner-all ui-shadow ui-btn-up-b" >
            <span class="ui-btn-inner ui-btn-corner-all">
                <span class="ui-btn-text">Nueva</span>
            </span>
        </a>
        <button id="butSearch" type="submit" data-theme="b">Buscar</button>
    </div>

    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="activities" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
        <input type="hidden" name="Code_User" id="hidCodeUser" value="<?php echo $oSite->get_logged_user_code();?>" />
<?php
    $oListHeader = new HelperListHeader("Fecha","Código",array("Contacto","Comercial"));

    $oDateFecha = new HelperDate("sch_DateP", $oActivitySearch->get_datep());
    $oDateFecha->set_extras("data-type=\"search\" placeholder=\"Fecha\"");
    $oDateFecha->set_class("ui-input-text ui-body-d");

    $oTxtContactDescription = new HelperText("sch_User_Description",$oActivitySearch->get_user_description());
    $oTxtContactDescription->set_extras("data-type=\"search\" placeholder=\"Nombre comercial\"");
    $oTxtContactDescription->set_class("ui-input-text ui-body-d");
    
//SHOW
$oDateFecha->show();
$oTxtContactDescription->show();
?>
        <br />
<?php
$oSelPage->show();
$oHidNumPage->show();
?>   
        <ul data-role="listview" data-inset="true">
<?php
$oListHeader->show();
$oHlpList->show();
?>
        </ul> 
     
    </div>
<?php
$oSelPage->show();
?>
</form>