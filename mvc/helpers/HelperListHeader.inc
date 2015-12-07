<?php
class HelperListHeader extends MainHelper
{
    private $_showAsColumns = true;
    private $_arColumnNames = array();
    private $_data_theme = "";
    private $_txtTopLeft = "";
    private $_txtTopRight = "";
     
    public function __construct($sLeftTop="",$sRightTop="", $arColumnNames=array())
    {
        parent::__construct();
        $this->_class = "ui-li ui-li-static ui-body-c ui-corner-top";
        $this->_txtTopLeft = $sLeftTop;
        $this->_txtTopRight = $sRightTop;
        $this->_arColumnNames = $arColumnNames;
    }
    
    private function get_html_li_header_as_brief()
    {
        $arColumnNames = $this->_arColumnNames;
        $sHtmlLi = "<li>\n";
        $sHtmlLi .= "<h3>$this->_txtTopLeft</h3>\n";
        $sHtmlLi .= "<p><strong>$this->_txtTopRight</strong></p>\n";
        foreach($arColumnNames as $sText)
            $sHtmlLi .= "<p>$sText</p>\n";
        $sHtmlLi .= "</li>";
        return $sHtmlLi;        
    }

    private function get_html_li_header_as_columns()
    {
        $arColumnNames = $this->_arColumnNames;
        $iWidth = $this->get_percent($arColumnNames);
        $sHtmlLi = "<li data-theme=\"$this->_data_theme\" class=\"$this->_class\">\n";
        $sHtmlLi .= "<h3 style=\"padding:0px 0px 10px 0px;\">$this->_txtTopLeft</h3>\n";
        //Código
        $sHtmlLi .= "<p class=\"ui-li-aside\"><strong>$this->_txtTopRight</strong></p>\n";
        foreach($arColumnNames as $sText)
            $sHtmlLi .= "<p style=\"padding:0; margin-top:-15px; display:inline; float:left; width:$iWidth%\">$sText
                        </p>\n";
        $sHtmlLi .= "</li>\n";
        return $sHtmlLi;
    }
    
    private function get_percent($arItems,$iNumItems="")
    {
        $iValues = $iNumItems;
        if(empty($iNumItems)) $iValues = count($arItems);
        if($iValues==0) $iValues=1;
        $fWidth = 100 / $iValues;
        $fWidth = number_format($fWidth, 2);
        return $fWidth;
    }
    
    public function show_li_header()
    {
        echo $this->get_html_li_header();
    }
    
    public function get_html()
    {
        if($this->_showAsColumns) return $this->get_html_li_header_as_columns();
        else return $this->get_html_li_header_as_brief();
    }
    
    public function not_show_as_columns($isOn=false)
    {
        $this->_showAsColumns = $isOn;
    }

}