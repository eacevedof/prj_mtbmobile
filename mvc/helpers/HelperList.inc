<?php
class HelperList extends MainHelper
{
    private $_arRows = array();
    private $_iPage = 1;
    private $_iNumPages = 1;
    private $_iTotalRegs = 0;
    private $_arKeysNames = array();
    private $_sH3FieldName = "";
    private $_arFieldsToShow = array();
    private $_anchor_extras = "";
    private $_arParamsFields = array();
    private $_useTiptextInP = false;
    private $_useTipTextInH3 = false;
    private $_anchor_nohref = false;
    private $_arLabels = array();
    private $_showAsColumns = true;
    
    /**
     *
     * @param string $sModule Nombre del módulo al que apuntara el link generado en el anchor
     * @param string $arRows Array de datos (filas) que se utilizarán para generar el listado
     * @param array $arKeysNames Los campos que son clave y que se desean mostrar en el listado (zona der sup negrita)
     * @param string $arFieldNamesInLine Los nombres de los campos cuyos valores se desean visualizar en la fila
     * @param string $sBoldFieldName El nombre del campo cuyo valor se mostrara como descripcion principal de la fila
     * @param int $iPage La página en la que se encuentra. Para mostralo en el "list divider"
     * @param int $iNumPages El total de páginas. Para mostrarlo en el "list divider"
     * @param int $iTotalRegs El total de Registros. Para mostrarlo en el "list divider"
     * @param string $sTab Metodo a ejecutar en el módulo indicado en el parametro $sModule
     * @param array $arParamsInLink Nombes de los campos que se desean añadir como parametros get en el atributo href
     */
    public function __construct($arRows=array(), $sBoldFieldName="", $arKeysNames=array(),
            $arFieldNamesInLine=array(), 
            $iPage=1,$iNumPages=1, $iTotalRegs=0,
            $sModule="clients", $sTab="detail", $arParamsInLink=array()
            )
    {
        parent::__construct();
        $this->_module = $sModule;
        $this->_tab = $sTab;
        $this->_arRows = $arRows;
        //La primera vez que se liste $iPage estara en null, se asume que es la primera
        $this->_iPage = $iPage;
        if(empty($this->_iPage)) $this->_iPage = 1;
        $this->_iNumPages = $iNumPages;
        $this->_iTotalRegs = $iTotalRegs;
        $this->_arKeysNames = $arKeysNames;
        $this->_sH3FieldName = $sBoldFieldName;
        $this->_arFieldsToShow = $arFieldNamesInLine;
        $this->_arParamsFields = $arParamsInLink;
        
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
        $sTxtStrong = ComponentText::clean_for_html($sTxtStrong);
        $sHtmlP = "<p";
        if($this->_showAsColumns) $sHtmlP .= " class=\"ui-li-aside\" style=\"margin:-5px 10px 0px 0px;\"";
        else $sHtmlP .= " style=\"white-space:normal;\"";
        $sHtmlP .= ">\n";
        $sHtmlP .= "<strong>$sTxtStrong</strong>\n";
        $sHtmlP .= "</p>\n";
        return $sHtmlP;
    }

    private function get_html_main_tag_h3($sH3Text)
    {
        $sHtmlH3 = ComponentText::clean_for_html($sH3Text);
        $sHtmlH3 = "<h3>$sH3Text</h3>";
        if($this->_useTipTextInH3) 
            $sHtmlH3="<h3><abbr title=\"$sH3Text\">$sH3Text</abbr></h3>\n";
        return $sHtmlH3;
    }

    private function get_html_p_field_value($sWidth,$sValue)
    {
        $sValue = ComponentText::clean_for_html($sValue);
        $sHtmlP = "<p";
        if($this->_showAsColumns) $sHtmlP .= " style=\"margin-top:1px; display:inline; float:left; width:$sWidth%;\"";
        else $sHtmlP .= " style=\"white-space:normal;\"";
        $sHtmlP .= ">\n";
        if($this->_useTiptextInP)
            $sHtmlP .= "<abbr title=\"$sValue\">$sValue</abbr>";
        else
            $sHtmlP .= $sValue;
        $sHtmlP .= "</p>\n";
        return $sHtmlP;
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
    
    private function get_html_p_field_values($arValues=array())
    {   
        //Ancho a asignar en cada item
        $fWidth = $this->get_percent($arValues);
        $sHtmlP = "";
        foreach($arValues as $key=>$sValue)
            $sHtmlP .= $this->get_html_p_field_value($fWidth, $sValue);

        return $sHtmlP;
    }

    public function get_html_li_header()
    {
        $arLabels = $this->_arLabels;
        $iNumItems = count($arLabels);
        $iWidth = $this->get_percent(null,$iNumItems);
        $sHtmlLi = "<li data-theme=\"\" class=\"ui-li ui-li-static ui-body-c ui-corner-top\">\n";
        $sHtmlLi .= "<p class=\"ui-li-aside ui-li-desc\"><strong></strong></p>\n";
        $sHtmlLi .= "<h3 style=\"padding:0px 0px 15px 0px;\" class=\"ui-li-heading\">$sText</h3>\n";
        foreach($arLabels as $i=>$sText)
        {
            $sHtmlLi .= "<p style=\"margin-top: -15px; display:inline-block; float:left; width:$iWidth%\" class=\"ui-li-desc\">\n
                            <strong>$sText</strong>
                        </p>\n";
        }
        $sHtmlLi .= "</li>";
        return $sHtmlLi;
    }
    
    public function show_li_header()
    {
        echo $this->get_html_li_header();
    }
    
    public function get_html()
    {
        $iNumRows = $this->_iTotalRegs;
        $sHtmlLiRows = "";
        $sHtmlLiRows .= $this->get_html_divider($iNumRows, $this->_iPage, $this->_iNumPages);
        
        if(is_array($this->_arRows))
            foreach($this->_arRows as $arFields)
            {
                $arLinkParams = $this->extract_fields_and_values($arFields, $this->_arParamsFields);
                if(!empty($this->_arKeysNames))
                    $sStrong = $this->extract_values($arFields, $this->_arKeysNames);
                $sH3Text = $arFields[$this->_sH3FieldName];
                $arValues = $this->extract_values($arFields, $this->_arFieldsToShow, 1);
                //bug($arValues); die;
                $sHtmlLiRows .= "<li data-inline=\"true\" >\n";
                if(!$this->_anchor_nohref) $sHrefPath = $this->get_full_uri_to_nav($this->_module, $this->_tab, $arLinkParams);
                
                $sHtmlLiRows .= "<a href=\"$sHrefPath\" ";
                //$sHtmlLiRows .= "data-ajax=\"false\" "; //No hace falta pq lo desactivo desde el js de inicio
                if($this->_anchor_nohref) $sHtmlLiRows .= "style=\"cursor:default\" ";
                if(!empty($this->_anchor_extras)) $sHtmlLiRows .= $this->_anchor_extras;
                $sHtmlLiRows .= ">\n";
                
                $sHtmlLiRows .= $this->get_html_main_tag_h3($sH3Text);
                $sHtmlLiRows .= $this->get_html_p_strong($sStrong);
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
    
    public function use_tiptext_in_p($isOn=true)
    {
        $this->_useTiptextInP = $isOn;
    }
    public function use_tiptext_in_h3($isOn=true)
    {
        $this->_useTipTextInH3 = $isOn;
    }
    
    public function anchor_nohref($isOn=true)
    {
        $this->_anchor_nohref = $isOn;
    }    
    
    public function set_header_labels($arLabels)
    {
        $this->_arLabels = $arLabels;
    }
    
    public function get_header_labels()
    {
        return $this->_arLabels;
    }
    
    public function not_show_as_columns($isOn=false)
    {
        return $this->_showAsColumns = $isOn;
    }
    
}