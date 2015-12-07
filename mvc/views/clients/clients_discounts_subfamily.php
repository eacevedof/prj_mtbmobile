<?php
//$oSelPage = new HelperSelect();
$oSchDiscount = $oSite->get_from_session("oSchClientDiscount");
if(empty($oSchDiscount))$oSchDiscount = new ModelMtbDescuentos();

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
//Barra del propietario
$oOwnerInfoBar->show();
//Botones foreign del propietario
$oFrgLink->show();
//Action bar
$oActionBar->show();

//$sUrl = "ABONOS SIN CODIFICACION";
//bug(urlencode($sUrl),$sUrl);
?>
<div id="divDialog"></div>

<form id="frmSearch" name="frmSearch" action="" method="post">

    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <button id="butSearch" type="submit" data-theme="b">Buscar</button>
    </div>

    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="descuentos" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
        <input type="hidden" name="Code_Familia" id="hidCodeFamilia" value="<?php echo $sCodeFamilia;?>" />
        <input type="hidden" name="Familia_Description" id="hidFamiliaDescription" value="<?php echo $sFamiliaDescription;?>" />
        <input type="hidden" name="Type" id="hidType" value="subfamilia" />
<?php
$oTxtSubfamilia = new HelperText("sch_Subfamilia_Description",$oSchDiscount->get_descripcion_subfamilia());
$oTxtSubfamilia->set_extras("data-type=\"search\" placeholder=\"Subfamilia\"");
$oTxtSubfamilia->set_class("ui-input-text ui-body-d");
$oTxtSubfamilia->class_not_for_label();

$oListHeader = new HelperListHeader("Subfamilia","Código",array("Dto."));

$oTxtSubfamilia->show();
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
