<?php
//$oSelPage = new HelperSelect();
$oSchProporsal = $oSite->get_from_session("oSchClientProporsal");
if(empty($oSchProporsal))$oSchProporsal = new ModelMtbPropuestaComercial();

$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());
//Barra del propietario
$oOwnerInfoBar->show();
//Botones foreign del propietario
$oFrgLink->show();
//Action bar
$oActionBar->show();
?>
<div id="divDialog"></div>

<form id="frmSearch" name="frmSearch" action="" method="post">
    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <button id="butSearch" type="submit" data-theme="b">Buscar</button>
    </div>
    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="propuestas" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidTab" value="<?php echo $sCodeAccount;?>" />
<?php
    $arPicklist = array(""=>"","Familia"=>"Familia","Subfamilia"=>"Subfamilia","Producto"=>"Producto");
    $oSelConcepto = new HelperSelect($arPicklist, "sch_Concepto","Concepto",$oSchProporsal->get_concepto());
    $oSelConcepto->set_extras("data-type=\"search\" placeholder=\"Concepto\"");
    $oSelConcepto->set_class("ui-input-text ui-body-d");
    $oSelConcepto->class_not_for_label();

    $oTxtCodProducto = new HelperText("sch_Code_Articulo",$oSchProporsal->get_code_articulo());
    $oTxtCodProducto->set_extras("data-type=\"search\" placeholder=\"Cod. Producto\"");
    $oTxtCodProducto->set_class("ui-input-text ui-body-d");
    
    $arPicklist = array(); $sCodeFamilia = $oSchProporsal->get_code_familia();
    $arPicklist = MainModel::get_data_for_picklist("MTB_Products_Family");
    $oSelFamilia = new HelperSelect($arPicklist,"sch_Code_Familia","Familia",$sCodeFamilia);
    $oSelFamilia->set_extras("data-type=\"search\"");
    $oSelFamilia->set_js_onchange("oMtbAjax.load_subfamilies(this);");
    $oSelFamilia->set_class("ui-input-text ui-body-d");
    $oSelFamilia->class_not_for_label();
    
    $arPicklist = array(); $sSQLAnd = "";
    if(!empty($sCodeFamilia)) $sSQLAnd = "Code_Family='$sCodeFamilia'";
    $arPicklist = MainModel::get_data_for_picklist("MTB_Products_SubFamily","","",$sSQLAnd);
    $oSelSubFamilia = new HelperSelect($arPicklist,"sch_Code_Subfamilia","Subfamilia",$oSchProporsal->get_code_subfamilia());
    $oSelSubFamilia->set_extras("data-type=\"search\"");
    $oSelSubFamilia->set_class("ui-input-text ui-body-d");
    $oSelSubFamilia->class_not_for_label();
    
    $oTxtCodProducto->show();
    $oSelConcepto->show();
    $oSelFamilia->show();
    $oSelSubFamilia->show();
?>
        <br />
<?php 
    $oListHeader = new HelperListHeader("Concepto","Cod. Prop",array("Item","Estado","Dto","Dto Ant"));
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
</form>
<!--/frmSearch-->
