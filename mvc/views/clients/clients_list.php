<?php
$oClientSearch = $oSite->get_from_session("oClientSearch");
if($oClientSearch==null) $oClientSearch = new ModelClients();
$oHidNumPage = new HelperHidden("hidNumPage", $oSelPage->get_selected_value());

//HTML
$oHlpActionBar->show();
?>
<form id="frmSearch" action="" method="post" class="ui-body-c ui-corner-all buttons-bar" style="margin:5px;">

    <div data-role="controlgroup" data-type="horizontal" style="text-align:center;">
        <a href="index.php?module=clients&tab=detail" data-inline="true" data-theme="b" data-role="button" class="btn-menu ui-btn ui-btn-inline ui-btn-corner-all ui-shadow ui-btn-up-b" >
            <span class="ui-btn-inner ui-btn-corner-all">
                <span class="ui-btn-text">Nuevo</span>
            </span>
        </a>
        <button id="butSearch" type="submit" data-theme="b">Buscar</button>
    </div>
    <!-- divlist -->
    <div class="content-primary">
        <!-- search fields -->
        <input type="text" id="sch_Name" name="sch_Name" placeholder="Nombre" value="<?php echo $oClientSearch->get_name(); ?>" data-type="search" class="ui-input-text ui-body-d" />
        <input type="text" id="sch_Codigo_Sic" name="sch_Codigo_Sic" placeholder="Nif/Cif" value="<?php echo $oClientSearch->get_codigo_sic(); ?>" data-type="search" class="ui-input-text ui-body-d" />
        <input type="text" id="sch_Cp" name="sch_Cp" placeholder="C.P." value="<?php echo $oClientSearch->get_cp(); ?>" data-type="search" class="ui-input-text ui-body-d" />
        <input type="text" id="sch_Poblacion" name="sch_Poblacion" placeholder="Población" value="<?php echo $oClientSearch->get_mtb_poblacion(); ?>" data-type="search" class="ui-input-text ui-body-d" />
        <!-- /search fields -->
        <br />
<?php
$oListHeader = new HelperListHeader("Nombre","Código",array("Población","C.P.","Vendedor"));
$oSelPage->show();
$oHidNumPage->show();
?>
        <ul data-role="listview" data-inset="true">
<?php
//LISTADO
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
<!--/list_clients.php-->