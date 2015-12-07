<?php
define("DS","/");
define("WS","/");
# ################################## #
#      ARCHIVO DE CONFIGURACION      #
#          telynetsales.com          #
# ################################## #
//define("DB_HOST","192.168.3.96");
define("DB_HOST","192.168.1.128\EALEXEIPXP");
define("DB_NAME","crm3_mtbkey");
define("DB_USER","sa");
define("DB_PASS","Sa2005");

define("JQ_DOMAIN_URI","192.168.1.128:8888/proy_mtbmobile/www");
//define("JQ_PROY_FOLDER_PATH","C:/Inetpub/wwwroot/proy_mtbmobile/");
define("JQ_PROY_FOLDER_PATH","C:/xampp/htdocs/proy_mtbmobile/");
# ################################## #
#             DEBUG VARS             #
# ################################## #
define("IS_DEBUG_SQL",0);
define("IS_DEBUG_MSG",0);
define("SHOW_PHP_INFO",0);

/*
 * COMPROBAMOS SI EL ACCESO ES DESDE DISPOSITIVO MOVIL
 */
//$_isMobile = "iphone";//eregi('ipod|iphone|ipad|android|opera mini|blackberry|palm os|windows ce|windows phone|opera mobi', $_SERVER['HTTP_USER_AGENT'] );

//URIS
define("JQ_DOMAIN_URIWS",JQ_DOMAIN_URI.WS);
define("JQ_DOMAIN_URI_HTTP","http://".JQ_DOMAIN_URI);
define("JQ_DOMAIN_URI_HTTPWS",JQ_DOMAIN_URI_HTTP.WS);
define("JQ_DOMAIN_URI_HTTPWS_INDEXPHP",JQ_DOMAIN_URI_HTTP.WS."index.php");
define("JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE",JQ_DOMAIN_URI_HTTPWS_INDEXPHP."?module=");