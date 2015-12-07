<?php
class HelperForeignLinks extends MainHelper
{
    private $_active_by_module = "";
    private $_active_by_description = "";
    private $_code_account = "";
    private $_arAccountFrgLinks = array();

    public function __construct($arModuleTabButtons=array(),$sCodeAccount="",$sActiveModule="",$sActiveDescription="")
    {
        parent::__construct();
        $this->_arAccountFrgLinks = $arModuleTabButtons;
        $this->_code_account = $sCodeAccount;
        $this->_active_by_module = $sActiveModule;
        $this->_active_by_description = $sActiveDescription;
    }

    public function get_html()
    {
        $sHtml = "";
        $sHtml .= "<div data-role=\"navbar\" data-theme=\"e\" >\n";
        $sHtml .= $this->get_html_ul();
        $sHtml .= "</div>\n";
        return $sHtml;
    }
  
    
    
    private function get_link($sModule,$tab="list",$arParams=array())
    {
        $sParams = $this->build_uri_params_with_keys($arParams);
        if(!empty($sParams)) $sParams="&$sParams";
        $sUrlLink = JQ_DOMAIN_URI_HTTPWS_INDEXPHP_PARAMS_MODULE."$sModule&tab=$tab&Code_Account=$this->_code_account$sParams";
        return $sUrlLink;
    }
    
    private function get_html_ul()
    {
        $arLinks = $this->_arAccountFrgLinks;
        $sHtmlUl = "";
        $sHtmlUl .= "<ul>\n";
        foreach ($arLinks as $arLink) 
        {
            $sTab = $arLink["tab"];
            if(empty($sTab))$sTab="list";
            $arParams = $arLink["params"];
            if(empty($arParams))$arParams = array();
            $sLink = $this->get_link($arLink["module"],$sTab,$arParams);
            $sDescription = $arLink["description"];
            
            $sHtmlAnchor = "<a href=\"$sLink\" ";
            if(!empty($this->_active_by_description))
            {    
                if($arLink["description"] == $this->_active_by_description) $sHtmlAnchor .= "class=\"ui-btn-active\" ";
            }
            elseif(!empty($this->_active_by_module))
            {        
                if($arLink["module"] == $this->_active_by_module) $sHtmlAnchor .= "class=\"ui-btn-active\" ";
            }
            
            
            $sHtmlAnchor .= ">\n";
            $sHtmlAnchor .= $sDescription;
            $sHtmlAnchor .= "</a>\n";
            $sHtmlUl .= "<li>$sHtmlAnchor</li>\n";
        }
        $sHtmlUl .= "</ul>\n";
        return $sHtmlUl;
    }
    
    public function set_active_module($sModule)
    {
        $this->_active_by_module = $sModule;
    }
    
    public function set_active_description($sDescription)
    {
        $this->_active_by_description = $sDescription;
    }
    
    public function set_code_account($sCodeAccount)
    {
        $this->_code_account = $sCodeAccount;
    }
}

/*
 * <!-- navbar -->
<div data-role="navbar">
    <ul>
        <li><a href="index.php?module=activities&tab=list&Code_Account=<? echo $oClient->get_code(); ?>" class="ui-btn-active">Actividades</a></li>
        <li><a href="index.php?module=contacts&tab=list&Code_Account=<? echo $oClient->get_code(); ?>" class="ui-btn-active">Contactos</a></li>
        <li><a href="index.php?module=historial&tab=list&Code_Account=<? echo $oClient->get_code(); ?>" class="ui-btn-active">Cond. Comerciales</a></li>
        <li><a href="index.php?module=historial&tab=list&Code_Account=<? echo $oClient->get_code(); ?>" class="ui-btn-active">Historial</a></li>
        <li><a href="index.php?module=historial&tab=list&Code_Account=<? echo $oClient->get_code(); ?>" class="ui-btn-active">Prop. Comercial</a></li>
    </ul>
</div>
<!-- \navbar -->
 */
