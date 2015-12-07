<?php
ob_start();
require_once("../config.inc");
require_once("folders_tree.php");
require_once("ZZZBootstrap.inc");
session_start();
Debug::config(IS_DEBUG_SQL, IS_DEBUG_MSG, SHOW_PHP_INFO);
$oDB = Database::get_instancia(DB_HOST,DB_NAME,DB_USER,DB_PASS,"sqlserver");
//bug($oDB);
$oAuthorization = new Authorization(); 
//bug($oAuthorization);
/**
 * @var GlobalParams
 */
$oGParams = new GlobalParams(); 
/**
 * @var Site
 */
$oSite = new Site();
//bug($_POST,"POST DESPUES DE CONST SITE"); 
$sControllerFile = $oSite->get_controller_file_name();
//bug($sControllerFile,"ARCHIVO DE CONTROLADOR"); DIE;
if(file_exists_in_incpath($sControllerFile))
{
    require_once $sControllerFile;
    //bug(get_included_files());
    $sControllerName = $oSite->get_controller_name();
    $sControllerMethod = $oSite->get_controller_method_name();
    Debug::set_message($sControllerName, "ControllerName");
    Debug::set_message($sControllerMethod,"ControllerMethod");
    $oController = new $sControllerName();
    if(!method_exists($oController,$sControllerMethod))
    {
        $sMessage="No se ha podido instanciar el metodo $sControllerName -> $sControllerMethod";
        Debug::set_message($sMessage);
    }
    else
    {
        $oController->$sControllerMethod();
        if($oSite->is_using_ajax())
        {
            //Esto evita que se cree el resto de código html
            exit();
        }
    }
}
//Archivo del controlador no existe
else
{
    $sMessage="El archivo $sControllerFile no existe en las rutas incluidas";
    Debug::set_message($sMessage);
}

if(Debug::is_messages_on()||Debug::is_php_info_on()||Debug::is_sqls_on())
{
    echo "<!-- debug -->\n<div id=\"divDebug\" ><center>";
    Debug::get_sqls_in_html_table();
    echo "<br />";
    Debug::get_messages_in_html_table();
    echo "<br />";
    Debug::get_php_info();
    echo "</center></div><!--/debug-->";
}
ob_flush();
