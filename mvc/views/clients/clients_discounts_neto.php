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
        <input type="hidden" name="Code_Subfamilia" id="hidCodeSubfamilia" value="<?php echo $sCodeSubFamilia;?>" />
        <input type="hidden" name="Articulo_Description" id="hidArticuloDescription" value="<?php echo $sProductoDescription;?>" />
        <input type="hidden" name="Type" id="hidType" value="neto" />    
<?php
    $oTxtCodProducto = new HelperText("sch_Code_Articulo",$oSchDiscount->get_code_articulo());
    $oTxtCodProducto->set_extras("data-type=\"search\" placeholder=\"Cod. Producto\"");
    $oTxtCodProducto->set_class("ui-input-text ui-body-d");
    $oTxtCodProducto->class_not_for_label();

    $oTxtProducto = new HelperText("sch_Producto_Description",$oSchDiscount->get_descripcion_producto());
    $oTxtProducto->set_extras("data-type=\"search\" placeholder=\"Producto\"");
    $oTxtProducto->set_class("ui-input-text ui-body-d");
    $oTxtProducto->class_not_for_label();

    $oListHeader = new HelperListHeader("Producto","Código",array("Producto","Cantidad","Precio"));

//SHOWS    
$oTxtCodProducto->show();
$oTxtProducto->show();
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