<?php
//$oSelPage = new HelperSelect();
$oActivitySearch = $oSite->get_from_session("oSchClientActivity");
if(empty($oActivitySearch))$oActivitySearch = new ModelActivities(null,null);

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
//Barra del propietario
$oOwnerInfoBar->show();
//Botones foreign del propietario
$oFrgLink->show();
//Action bar
$oActionBar->show();
?>
<form id="frmSearch" name="frmSearch" action="" method="post">
    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <button id="butSearch" type="submit" data-theme="b" value="search">Buscar</button>
    </div>
    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="activities" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
        <input type="hidden" name="Code_Result" id="hidCodeResult" value="1" />

<?php
$oListHeader = new HelperListHeader("Fecha","Código",array("Contacto","Comercial"));

$oDateFecha = new HelperDate("sch_DateP", $oActivitySearch->get_datep());
$oDateFecha->set_extras("data-type=\"search\" placeholder=\"Fecha\"");
$oDateFecha->set_class("ui-input-text ui-body-d");

$oTxtContactDescription = new HelperText("sch_Contact_Description",$oActivitySearch->get_contact_description());
$oTxtContactDescription->set_extras("data-type=\"search\" placeholder=\"Contacto\"");
$oTxtContactDescription->set_class("ui-input-text ui-body-d");

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
