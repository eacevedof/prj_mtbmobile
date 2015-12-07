<?php
class HelperListPopup extends MainHelper
{
    private $_arDataList = array();
    private $_iPage = 1;
    private $_iNumPages = 1;
    private $_iTotalRegs = 0;
    private $_arKeysNames = array();
    private $_sH3FieldName = "";
    private $_arLabels = array();
    private $_anchor_extras = "";
    private $_anchor_class = "";
    private $_anchor_nohref = false;
    
    public function __construct($code_module,
            $arDataList=array(),$arKeysNames=array(), $arFieldNamesInLine=array(), 
            $sH3FieldName="", $iPage=1,$iNumPages=1, $iTotalRegs=0, $code_tab="detail", $sAnchorClass="clsPopup")
    {
        parent::__construct();
        $this->_code_module = $code_module;
        $this->_code_tab = $code_tab;
        $this->_arDataList = $arDataList;
        //La primera vez que se liste $iPage estara en null, se asume que es la primera
        $this->_iPage = $iPage;
        if(empty($this->_iPage)) $this->_iPage = 1;
        $this->_iNumPages = $iNumPages;
        $this->_iTotalRegs = $iTotalRegs;
        $this->_arKeysNames = $arKeysNames;
        $this->_sH3FieldName = $sH3FieldName;
        $this->_arLabels = $arFieldNamesInLine;
        $this->_anchor_class = $sAnchorClass;
        
        //bug($this,"this");
    }
    
    private function get_html_divider($iNumRows="", $iPagina="", $iNumPaginas="")
    {
        $sHtmlLi = "<li data-role=\"list-divider\" >\n";
        $sHtmlLi .= "<span class=\"ui-li-count\">Resultado: $iNumRows Regs. ";
        if($iNumRows>0)
            $sHtmlLi .= "- Pag. $iPagina / $iNumPaginas ";
        $sHtmlLi .= "</span>\n";
        $sHtmlLi .= "</li>\n";
        return $sHtmlLi;
    }

    private function get_html_p_strong($sTxtStrong)
    {
        $sHtmlP = "<p class=\"ui-li-aside\" style=\"margin:-5px 10px 0px 0px;\">
                        <strong>$sTxtStrong</strong>
                    </p>";
        return $sHtmlP;
    }

    private function get_html_main_tag_h3($sH3Text)
    {
        $sHtmlH3 = "<h3>$sH3Text</h3>";
        return $sHtmlH3;
    }

    private function get_html_p_field_value($sWidth,$sValue)
    {
        $sHtmlP = "<p style=\"margin-top:1px; display:inline; float:left; width:$sWidth%;\">$sValue</p>\n";
        return $sHtmlP;
    }
    
    private function get_html_p_field_values($arValues=array())
    {
        $iValues = count($arValues);
        if($iValues==0) $iValues=1;
        $fWidth = 100 / $iValues;
        $fWidth = number_format($fWidth, 2);
        $sHtmlP = "";
        foreach($arValues as $key=>$sValue)
        {
            $sHtmlP .= $this->get_html_p_field_value($fWidth, $sValue);
        }
        return $sHtmlP;
    }

    public function get_html()
    {
        $iNumRows = $this->_iTotalRegs;
        $sHtmlLiRows = "";
        $sHtmlLiRows .= $this->get_html_divider($iNumRows, $this->_iPage, $this->_iNumPages);
        
        if(!empty($this->_arDataList))
            foreach($this->_arDataList as $iRow=>$arFields)
            {
                //$arKeysAndValues = $this->extract_fields_and_values($arFields, $this->_arKeysNames);
                if($this->_anchor_nohref)
                    $sHrefPath = "#";
                else
                    $sHrefPath = $this->get_full_uri_to_nav($this->_code_module, $this->_code_tab, $arFields);
                $sStrong = $this->extract_values($arFields, $this->_arKeysNames);
                $sH3Text = $arFields[$this->_sH3FieldName];
                $arValues = $this->extract_values($arFields, $this->_arLabels, 1);
                //bug($arValues); die;
                $sHtmlLiRows .= "<li data-inline=\"true\" >\n";
                $sHtmlLiRows .= "<a href=\"$sHrefPath\" ";
                $sHtmlLiRows .= "data-ajax=\"false\" ";
                if(!empty($this->_anchor_class)) $sHtmlLiRows .= "class=\"$this->_anchor_class\"";
                if(!empty($this->_anchor_extras)) $sHtmlLiRows .= $this->_anchor_extras;
                $sHtmlLiRows .= ">\n";
                $sHtmlLiRows .= $this->get_html_p_strong($sStrong);
                $sHtmlLiRows .= $this->get_html_main_tag_h3($sH3Text);
                $sHtmlLiRows .= $this->get_html_p_field_values($arValues);
                $sHtmlLiRows .= "</a>\n";
                $sHtmlLiRows .= "</li>\n";
            }
        return $sHtmlLiRows;
    }
    
    public function set_anchor_extras($sValue)
    {
        $this->_anchor_extras = $sValue;
    }
    
    public function set_anchor_class($sValue)
    {
        $this->_anchor_class = $sValue;
    }
    
    public function anchor_nohref($isOn=true)
    {
        $this->_anchor_nohref = $isOn;
    }
}