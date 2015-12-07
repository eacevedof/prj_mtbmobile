<?php
class ViewDefault 
{
    private $_arFileNames = array();
    private $_sFileHtmlHead = "html_head.inc"; //Apertura html, body y div page
    private $_sFileDivPageHeader = "div_page_header.php"; //Apertura de div content
    private $_sFileInfoBar = "info_bar.php";
    private $_sFixedMenuBar = "fixed_menu_bar.php";
    private $_sFileDivPageFooter = "div_page_footer.php"; //Cierra div content, body y html
    
    public function __construct()
    {}
  
    public function add_file($sViewFile)
    {
        $this->_arFileNames[] = $sViewFile;
    }
    
    public function display()
    {   
        
        /**
         * @type GlobalParams
         */
        global $oGParams; 
        global $oSite;
        
        //bug($_POST); bug($oSite); die();
        $arParams = $oGParams->get_vars();
        $arParams["oSite"] = $oSite;
        
        try
        {
            //Cargo todas las variables guardadas en Site->params para que
            //estén disponibles en todas las vistas
            foreach ($arParams as $sVarName => $mixVariable)
                $$sVarName = $mixVariable;

            /**/
            require($this->_sFileHtmlHead);
            require($this->_sFileDivPageHeader);
            require($this->_sFileInfoBar);
            require($this->_sFixedMenuBar);
            //Inserto las vistas
            foreach($this->_arFileNames as $sFileName)
                require($sFileName);

            require($this->_sFileDivPageFooter);
            //require($this->_sFileHtmlBody);
            $_GET = null; $_POST=null;
            /**/
        }
        catch(Exception $e)
        {
            die("<b>TPL Error</b>:" . $e->getMessage());
        }
    }
}