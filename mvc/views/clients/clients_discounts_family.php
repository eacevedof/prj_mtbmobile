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
        <button id="butSearch" type="submit" data-theme="b" value="search">Buscar</button>
    </div>

    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="descuentos" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?echo $sCodeAccount;?>" />
<?php
    $oTxtCodProducto = new HelperText("sch_Code_Articulo",$oSchDiscount->get_code_articulo());
    $oTxtCodProducto->set_extras("data-type=\"search\" placeholder=\"Cod. Producto\"");
    $oTxtCodProducto->set_class("ui-input-text ui-body-d");
    $oTxtCodProducto->class_not_for_label();
    
    $arPicklist = MainModel::get_data_for_picklist("MTB_Products_Family");
    $sCodeFamilia = $oSchDiscount->get_code_familia();
    $oSelFamilia = new HelperSelect($arPicklist,"sch_Code_Familia","Familia",$sCodeFamilia);
    $oSelFamilia->set_extras("data-type=\"search\" placeholder=\"Familia\"");
    $oSelFamilia->set_js_onchange("oMtbAjax.load_subfamilies(this);");
    $oSelFamilia->set_class("ui-input-text ui-body-d");
    $oSelFamilia->class_not_for_label();
    
    $sSQLAnd = "Code_Family='$sCodeFamilia'";
    $arPicklist = MainModel::get_data_for_picklist("MTB_Products_SubFamily","","",$sSQLAnd);
    $oSelSubFamilia = new HelperSelect($arPicklist,"sch_Code_Subfamilia","Subfamilia",$oSchDiscount->get_code_subfamilia());
    $oSelSubFamilia->set_extras("data-type=\"search\" placeholder=\"Subfamilia\"");
    $oSelSubFamilia->set_class("ui-input-text ui-body-d");
    $oSelSubFamilia->class_not_for_label();
    
    $oListHeader = new HelperListHeader("Familia","Código",array("Dto.","Subfamilia","Netos"));
    
//SHOWS
$oTxtCodProducto->show();
$oSelFamilia->show();
$oSelSubFamilia->show();
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
    <!-- divlist -->
</form><!--/jqForm-->
<!--/list_contacts.php-->