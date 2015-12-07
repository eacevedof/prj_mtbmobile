<?php
//$oSelPage = new HelperSelect();
$oActivitySearch = $oSite->get_from_session("oSchClientActivity");
if(empty($oActivitySearch))$oActivitySearch = new ModelActivities();

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
//Barra del propietario
$oOwnerInfoBar->show();
//Botones foreign del propietario
$oFrgLink->show();
//Action bar
$oActionBar->show();
?>
<form id="frmSearch" name="frmSearch" action="" method="post">
    <div id="divButtons" data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <a href="index.php?module=activities&tab=detail&Code_Account=<?php echo $sCodeAccount;?>" data-inline="true" data-theme="b" data-role="button" class="btn-menu ui-btn ui-btn-inline ui-btn-corner-all ui-shadow ui-btn-up-b">
            <span class="ui-btn-inner ui-btn-corner-all">
                <span class="ui-btn-text">Nueva</span>
            </span>
        </a>
        <button type="submit" id="butSearch" data-theme="b" >Buscar</button>
    </div>
    <!-- divlist -->
    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="activities" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
<?php
$oFecha = new HelperDate("sch_DateP",$oActivitySearch->get_datep());
$oFecha->set_extras("data-type=\"search\" placeholder=\"Fecha\"");
$oFecha->set_class("ui-input-text ui-body-d");

$oContactDescription = new HelperText("sch_Contact_Description",$oActivitySearch->get_contact_description());
$oContactDescription->set_extras("data-type=\"search\" placeholder=\"Contacto\"");
$oContactDescription->set_class("ui-input-text ui-body-d");

$oListHeader = new HelperListHeader("Fecha","Código",array("Contacto","Comercial"));

//SHOW
$oFecha->show();
$oContactDescription->show();
?>
<br />
<?php
//SELECT PAGINA
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
    <!-- divlist -->
</form><!--/jqForm-->
<!--/list_contacts.php-->