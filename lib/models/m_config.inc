<?php
//Model configuration
class ModelConfiguration extends MainModel
{

    public function __construct() 
    {
        global $oDB;
        $this->oDB = $oDB;
        $this->_table_name ="m_config";
        $this->load_as_object();
    }
    
     
    protected function load_as_object()
    {
        $sSQL = "SELECT *
                 FROM $this->_table_name";
        $arRows = $this->oDB->query($sSQL);
        foreach($arRows as $arRow)  
        {
            switch($arRow["Type"]) 
            {
                case "TEXT":
                case "VAR":
                default:
                    $this->{$arRow["Code"]} = $arRow["Value"];
            }
        }
    }    
    //===========================     METODOS     ===========================    

}