<?php
//$oSelPage = new HelperSelect();
$oMtbEstadistica = $oSite->get_from_session("oSchClientStadistics");
if(empty($oMtbEstadistica))$oMtbEstadistica = new ModelMtbEstadisticas();

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
        <button id="butSearch" type="submit" data-theme="b" value="search">Buscar</button>
    </div>

    <div class="content-primary">
        <input type="hidden" name="module" id="hidModule" value="ventas" />
        <input type="hidden" name="tab" id="hidTab" value="list" />
        <input type="hidden" name="Code_Account" id="hidCodeAccount" value="<?php echo $sCodeAccount;?>" />
<?php
$arPicklist = array
    (   ""=>"","todos"=>"* TODOS",
        "01"=>"Enero","02"=>"Febrero","03"=>"Marzo","04"=>"Abril","05"=>"Mayo","06"=>"Junio",
        "07"=>"Julio","08"=>"Agosto","09"=>"Septiembre","10"=>"Octubre","11"=>"Noviembre","12"=>"Diciembre"
    );
//bug($oMtbEstadistica->get_meses());
$oSelMeses = new HelperSelect($arPicklist,"sch_Meses","Meses",$oMtbEstadistica->get_meses(),true);
$oSelMeses->set_extras("data-type=\"search\" data-native-menu=\"false\"");
$oSelMeses->set_class("ui-input-text ui-body-d");
$oSelMeses->class_not_for_label();
$oSelMeses->set_js_onchange("on_sch_mes_change(this)");

$arPicklist = MainModel::get_data_for_picklist("MTB_Products_Family");
$arPicklist["todas"]="* TODAS";
$sCodeFamilia = $oMtbEstadistica->get_code_familia();
$oSelFamilia = new HelperSelect($arPicklist,"sch_Code_Familia","Familia",$sCodeFamilia);
$oSelFamilia->set_extras("data-type=\"search\" data-native-menu=\"true\"");
$oSelFamilia->set_js_onchange("oMtbAjax.load_subfamilies(this,null,'ventas');");
$oSelFamilia->set_class("ui-input-text ui-body-d");
$oSelFamilia->class_not_for_label();
//bug($oSelFamilia); die;

$sSQLAnd = "Code_Family='$sCodeFamilia'";
$arPicklist = MainModel::get_data_for_picklist("MTB_Products_SubFamily","","",$sSQLAnd);
$arPicklist["todas"]="* TODAS";
//bug($arPicklist);
$oSelSubFamilia = new HelperSelect($arPicklist,"sch_Code_Subfamilia","Subfamilia",$oMtbEstadistica->get_code_subfamilia());
$oSelSubFamilia->set_extras("data-type=\"search\" data-native-menu=\"true\"");
$oSelSubFamilia->set_class("ui-input-text ui-body-d");
$oSelSubFamilia->class_not_for_label();

//Selects search
$oSelMeses->show();
$oSelFamilia->show();
$oSelSubFamilia->show();
?>
<br />
<?php
//Select Páginas
$oSelPage->show();
$oHidNumPage->show();
?>            
        <ul data-role="listview" data-inset="true">
<?php
$oHlpList->show_li_header();
$oHlpList->show();
?>
        </ul> 
    </div>
<?php
//Select Paginas
$oSelPage->show();
?>        
    <!-- divlist -->
</form><!--/jqForm-->
<!--/list_contacts.php-->
