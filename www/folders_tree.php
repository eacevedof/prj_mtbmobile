<?php
//JQ_PROY_FOLDER_PATH = "C:/xampp/htdocs/proy_mtbmobile/";
//JQ_PROY_FOLDER_PATH = "E:/WebServer/proy_mtbmobile/";
$arPaths["pear"] = get_include_path();
$arPaths["project"] = JQ_PROY_FOLDER_PATH;
$arPaths["lib"] = JQ_PROY_FOLDER_PATH."lib/";
$arPaths["lib_models"] = JQ_PROY_FOLDER_PATH."lib/models/";
$arPaths["models"] = JQ_PROY_FOLDER_PATH."mvc/models/";
$arPaths["controllers"] = JQ_PROY_FOLDER_PATH."mvc/controllers/";
$arPaths["components"] = JQ_PROY_FOLDER_PATH."mvc/components/";
$arPaths["views"] = JQ_PROY_FOLDER_PATH."mvc/views/";
$arPaths["views_clients"] = JQ_PROY_FOLDER_PATH."mvc/views/clients/";
$arPaths["views_contacts"] = JQ_PROY_FOLDER_PATH."mvc/views/contacts/";
$arPaths["views_activities"] = JQ_PROY_FOLDER_PATH."mvc/views/activities/";
$arPaths["views_users"] = JQ_PROY_FOLDER_PATH."mvc/views/users/";
$arPaths["views_discounts"] = JQ_PROY_FOLDER_PATH."mvc/views/discounts/";
$arPaths["views_proporsals"] = JQ_PROY_FOLDER_PATH."mvc/views/proporsals/";
$arPaths["views_sales"] = JQ_PROY_FOLDER_PATH."mvc/views/sales/";
$arPaths["interfaces"] = JQ_PROY_FOLDER_PATH."mvc/interfaces/";
$arPaths["elements"] = JQ_PROY_FOLDER_PATH."mvc/elements/";
$arPaths["helpers"] = JQ_PROY_FOLDER_PATH."mvc/helpers/";
$sProjectPaths = join(";",$arPaths);
//bug($sProjectPaths,$sProjectPaths);
//foreach($arPaths as $sPath)
   // bugdir($sPath);

set_include_path($sProjectPaths);
