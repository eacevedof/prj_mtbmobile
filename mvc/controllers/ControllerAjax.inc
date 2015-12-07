<?php
class ControllerAjax extends MainController implements InterfaceController  
{
    public function __construct()
    {
        parent::__construct();
        //Indica que no se añadirán salidas como el debug al contenido devuelto 
        //del script general
        $this->oSite->use_ajax();
    }

    public function get_postal_codes_by_province()
    {
        //bug($_POST);
        $sCodeProvincia = $_POST["Code_Provincia"];
        //$sControlId = $_POST[""];
        $sSQLAnd = "Code_Provincia='$sCodeProvincia'";
        $arOptions = MainModel::get_data_for_picklist("MTB_Codigos_Postales","Code","Cp",$sSQLAnd);
        $oSelCodigosPostales = new HelperSelect($arOptions, "det_Cp");
        //bug($oSelCodigosPostales);
        //$oSelCodigosPostales->get_html();
        $oSelCodigosPostales->show();
    }
    
    public function get_manager_by_vendedor()
    {
        //bug($_POST);
        $sCodeVendedor = $_POST["Code_Vendedor"];
        //$sControlId = $_POST[""];
        $sSQLAnd = "Code=(SELECT Code_Manager FROM core_users WHERE Code='$sCodeVendedor')";
        $arOptions = MainModel::get_data_for_picklist("core_users","Code","Description",$sSQLAnd);
        //$oSelManager = new HelperSelect($arOptions, "Code_Manager","Manager");
        $oSelManager = new HelperSelect($arOptions, "Code_Manager");
        //bug($oSelManager);
        $oSelManager->readonly();        
        //bug($oSelManager);
        $oSelManager->show();
    }
    
    public function get_clientes_by_vendedor()
    {
        //bug($_POST);
        $sCodeVendedor = $_POST["Code_Vendedor"];
        //$sControlId = $_POST[""];
        $sSQLAnd = "Code IN (SELECT Code FROM clients WHERE Propietario='$sCodeVendedor')";
        $arOptions = MainModel::get_data_for_picklist("clients","Code","Name",$sSQLAnd);
        //$oSelManager = new HelperSelect($arOptions, "Code_Manager","Manager");
        $oSelClient = new HelperSelect($arOptions, "det_Code_Account");
        $oSelClient->set_js_onchange("oMtbAjax.load_contactos(this);");
        $oSelClient->show();        
    }
    
    public function get_contacts_by_client()
    {
        //bug($_POST);
        $sCodeAccount = $_POST["Code_Account"];
        //$sControlId = $_POST[""];
        $sSQLAnd = "Code_Account='$sCodeAccount'";
        $arOptions = MainModel::get_data_for_picklist("contacts","Code","Description",$sSQLAnd);
        //$oSelManager = new HelperSelect($arOptions, "Code_Manager","Manager");
        $oSelContact = new HelperSelect($arOptions, "det_Code_Contact");
        $oSelContact->show();           
    }

    public function get_subfamilies_by_family()
    {
        $sCodeFamily = $_POST["Code_Family"];
        $sModuleFrom = $_POST["modulefrom"];
        //if(!empty($sCodeFamily))
        $sSQLAnd = "Code_Family='$sCodeFamily'";
        $arPicklist = MainModel::get_data_for_picklist("MTB_Products_SubFamily",null,null,$sSQLAnd);
        if($sModuleFrom=="ventas")$arPicklist["todas"]="* TODAS";
        $oSelSubFamilia = new HelperSelect($arPicklist,"sch_Code_Subfamilia");
        $oSelSubFamilia->set_class("clsNone");
        $oSelSubFamilia->set_extras("data-type=\"search\" placeholder=\"Subfamilia\"");
        $oSelSubFamilia->show();
    }
    
    public function basic_list(){}
    public function detail(){}
    public function delete(){} 
}

