<?php
class GlobalParams
{
  
    private $_arMessages = array();
    private $_arVars = array();

    # Constructor
    public function __construct()
    { 
    }
    
    public function set_in_vars($sVarName,$mixVariable)
    {
        $this->_arVars[$sVarName] = $mixVariable;
    }
    
    public function get_vars()
    {
        //si no es static: Can't use method return value in write context in C:\xampp\htdocs\proy_mtbmobile\mvc\views\ViewDefault.inc on line 29
        return $this->_arVars;
    }
    
    public function get_var($key)
    {
        return $this->_arVars[$key];
    }
    
    public function get_messages_as_rray()
    {
        return $this->_arMessages;
    }
    
    public function get_messagess_as_string()
    {
        $arMessages = $this->_arMessages;
        $sReturn = "";
        foreach ($arMessages as $sMessage)
        {
            $sReturn .= "$sMessage \n";
        }
        return $sReturn;
    }
    
    public function set_message($sMessage)
    {
        $this->_arMessages[] = $sMessage;
    }
   
}
