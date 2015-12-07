<?php
class HelperDivHeader extends MainHelper
{
    private $_h1_title;

    public function __construct($sH1Title="")
    {
        parent::__construct();
        $this->_h1_title = $sH1Title;
    }

    public function get_html()
    {
        $sHtml = "";
        $sHtml .= "<div id=\"divActionBar\" data-role=\"header\" data-theme=\"b\" data-position=\"inline\">\n";
        if(!empty($this->_h1_title))
            $sHtml .= "<h1>$this->_h1_title</h1>\n";
        $sHtml .= "</div>\n"; 
        return $sHtml;
    }
    
    public function get_h1_title()
    {
        return $this->_h1_title;
    }
    
    public function set_h1_title($sValue)
    {
        $this->_h1_title = $sValue;
    }
}

/**
 <div id="divFormMenu" data-role="header" data-theme="b" data-position="inline">
    <h1>Modulo(objeto)</h1>
    <p>Nuevo ó Código</p>
    <a href="#" data-role="button" data-icon="search" data-iconpos="notext" class="ui-btn-right jqm-home">
        refrescar
    </a>
    <a href="#" data-role="button" data-icon="refresh" data-iconpos="notext" class="ui-btn-left jqm-home">
        refrescar
    </a>    
</div>
 */