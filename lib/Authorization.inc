<?php
/*
 * Created on 09/02/2012 by Pedro Alonso
 */
class Authorization
{
    /**
     * @var Database
     */
    private $oDB;
    private $_login_table = "core_users";
    private $_login = "";
    private $_password = "";

    private $_user_logged = array(); //datos del usuario en sesion

    # Constructor
    public function __construct()
    {
        global $oDB;
        if($this->is_logingout())
        {
            //Destruye la sesion
            $this->logout();
        }
        //$_GET["action"]!="logout"
        else
        {
            $this->oDB = $oDB;
            $this->_login = $_POST["txtLogin"];
            $this->_password = $_POST["pasPassword"];
            
            
            //Si no hay usuario en sesion
            if(!$this->is_user_logged())
                //Comprueba si el acceso es válido incluye vacios
                if($this->is_valid_login())
                {
                    $this->set_user_in_session();
                }
                else
                {
                    $this->_is_correct = false;
                    $this->_user_logged = array();
                    $this->logout();
                }
        }
    }
    
    private function is_logingout()
    {
        if($_GET["user_action"] == "exit")
        {
            return true;
        }        
        return false;
    }
    
    public function logout()
    {
        session_destroy();
        $_SESSION = array();
        $_GET = array();
        $_POST = array();
        $this->go_to_login();
    }
    
    public function go_to_login()
    {
        require_once("./login.php");
        exit();        
    }

    private function is_valid_login()
    {
        if(!empty($this->_login)&&!empty($this->_password))
        {
            $sPassword = md5($this->_password);
            $sSQL = "SELECT * 
                     FROM $this->_login_table 
                     WHERE Login='$this->_login' 
                     AND Password='$sPassword' AND Active='1'";
            
            //bug($this->oDB);die;
            $arUser = $this->oDB->query($sSQL);
            //bug($arUser); die;
            if(count($arUser)==1)
            {   
                $this->_user_logged = $arUser[0];
                return true;
            }
        }
        return false;
    }
    
    private function set_user_in_session()
    {
        session_start();
        $_SESSION["user_logged"] = $this->_user_logged; 
    }

    public function is_user_logged()
    {
        if(!empty($_SESSION["user_logged"]))
        {
            return true;
        }
        return false;        
    }
    

    public function get_user_logged()
    {
        return $this->_user_logged;
    }
    
    public function get_login_table()
    {
        return $this->_login_table;
    }
}

