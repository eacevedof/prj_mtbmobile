<?php
$oContactSearch = $oSite->get_from_session("oContactSearch");
if($oContactSearch==null) $oContactSearch = new ModelContacts();

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
//Barra del propietario
$oHlpOwnerInfoBar->show();
//Botones foreign del propietario
$oHlpFrgLink->show();
//Action bar
$oHlpActionBar->show();
?>
<form id="frmSearch" name="frmSearch" action="" method="post">
    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <a href="index.php?module=contacts&tab=detail&Code_Account=<?php echo $sCodeAccount;?>" data-inline="true" data-theme="b" data-role="button" class="btn-menu ui-btn ui-btn-inline ui-btn-corner-all ui-shadow ui-btn-up-b" >
            <span class="ui-btn-inner ui-btn-corner-all">
                <span class="ui-btn-text">Nuevo</span>
            </span>
        </a>
        <button id="butSearch" type="submit" data-theme="b" value="search">Buscar</button>
    </div>
  
    <!-- divlist -->
    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="contacts" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
<?php
$oTxtContacto = new HelperText("sch_Full_Name",$oContactSearch->get_full_name());
$oTxtContacto->set_extras("data-type=\"search\" placeholder=\"Nombre completo\"");
$oTxtContacto->set_class("ui-input-text ui-body-d");

$oTxtCargo = new HelperText("sch_Full_Name",$oContactSearch->get_cargo());
$oTxtCargo->set_extras("data-type=\"search\" placeholder=\"Cargo\"");
$oTxtCargo->set_class("ui-input-text ui-body-d");

$oListHeader = new HelperListHeader("Nombre Completo","Código",array("Teléfono","Cargo"));

$oTxtContacto->show();
$oTxtCargo->show();
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