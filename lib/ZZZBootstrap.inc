<?php
//echo "Bootstrap.inc";
require_once("PhpIniConfig.inc");

# Requirements (Common Classes)
require_once("Debug.inc");
require_once("Functions.inc");
require_once("Database.inc");
require_once("GlobalParams.inc");
require_once("Authorization.inc");
require_once("Site.inc");       //Site class

//Clases madres
require_once("MainModel.inc");
require_once("MainController.inc");
require_once("MainHelper.inc");
require_once("MainView.inc");
require_once("MainComponent.inc");  //libs
//Sort of namespace with models which represents m_tables
require_once("ns_models.inc"); 

require_once("InterfaceModel.inc");
require_once("InterfaceController.inc");
require_once("ViewDefault.inc");	//View printer

require_once("Translation.inc");//Translator
require_once("ComponentPaginate.inc");
require_once("ComponentUtility.inc");
require_once("ComponentText.inc");

//MY MODELS Evita  el __PHP_Incomplete_Class cuando guardo en session un objeto
require_once "ModelCoreUsers.inc";
require_once "ModelClients.inc";
require_once "ModelMtbClientes.inc";
require_once "ModelContacts.inc";
require_once "ModelMtbContactos.inc";
require_once "ModelActivities.inc";
require_once "ModelMtbActivities.inc";
require_once "ModelMtbDescuentos.inc";
require_once "ModelMtbPropuestaComercial.inc";
require_once "ModelMtbEstadisticas.inc";

//HELPERS Obligatorios
require_once "HelperActionBar.inc";
require_once "HelperOwnerInfoBar.inc";
require_once "HelperForeignLinks.inc";

require_once "HelperList.inc";
require_once "HelperListPopup.inc";
require_once "HelperSelect.inc";
require_once "HelperRadio.inc";
require_once "HelperJavascript.inc";
require_once "HelperHidden.inc";
require_once "HelperText.inc";
require_once "HelperDate.inc";
require_once "HelperSubmitButton.inc";
require_once "HelperTextArea.inc";
require_once "HelperDivHeader.inc";
require_once "HelperListHeader.inc";




