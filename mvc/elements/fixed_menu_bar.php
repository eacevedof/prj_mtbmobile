<?php
$sCodeUserSession = $oSite->get_logged_user_code();
$sLinkClientes = JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=clients&tab=list";
$sLinkEstadisticas = JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=estadisticas&tab=list";
$sLinkActividades = JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=activities&tab=list&Code_User=$sCodeUserSession";
?>
<!-- header_bar.php -->
<div id="divMainMenu" data-role="header" data-theme="a" data-position="fixed" data-iconpos="top">
    <div data-role="navbar" data-iconpos="top">
        <ul>
            <li><a href="<? echo $sLinkClientes; ?>" data-icon="home" >Clientes</a></li>
            <li><a href="<? echo $sLinkActividades; ?>" data-icon="grid" >Activid.</a></li>
            <!--<li><a href="<? echo $sLinkEstadisticas; ?>" data-icon="star" >Estad.Venta</a></li>-->
        </ul>        
    </div>
 </div>
 <!-- header_bar.php -->


     
 