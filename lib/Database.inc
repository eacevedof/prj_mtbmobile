<?php
/*
 * Created on 15/02/2012 by Pedro Alonso
 */
//include("DB.php"); //PEAR OBJECT DB
//ini_set("mssql.datetimeconvert",0);

class Database //Tipo Singleton
{
    private $_sServidor;
    private $_sNombreBD;
    private $_sUsuario;
    private $_sClave;
    private $_sTipoBD;
   
    private $_oLinkId;
    private $_sMensaje;
    private $_isError;
   
    /**
    * @var CBaseDatos
    */
    private static $_oSelf = null;
   
    private function __construct($sServidorBD="",$sNombreBD="",$sUserBD="",$sClaveBD="",$sTipoBD="mysql")
    {
        $this->_sServidor = $sServidorBD;
        $this->_sNombreBD = $sNombreBD;
        $this->_sUsuario = $sUserBD;
        $this->_sClave = $sClaveBD;
        $this->_sTipoBD = strtolower(trim($sTipoBD));
        $this->_oLinkId = null; //oId de Conexión
        $this->_sMensaje = "";
        $this->_isError = false;
    }
   
    /**
    * Este es el pseudo constructor singleton
    * Comprueba si la variable privada $_oSelf tiene un objeto
    * de esta misma clase, sino lo tiene lo crea y lo guarda
    * @return CBaseDatos
    */
    public static function get_instancia($sServidorBD="",$sNombreBD="",$sUserBD="",$sClaveBD="",$sTipo="mysql")
    {
        if(!self::$_oSelf instanceof self)
        {
            self::$_oSelf = new self($sServidorBD,$sNombreBD,$sUserBD,$sClaveBD,$sTipo);
        }
        return self::$_oSelf;
    }

    //=================== CONECTAR ===========================
    private function conectar_mysql()
    {
        $sNombreBD = $this->_sNombreBD;
        if($this->_oLinkId==0)
        {
            $oConnect = mysql_connect
            (
                $this->_sServidor,
                $this->_sUsuario,
                $this->_sClave
            );
            if(!is_resource($oConnect))
            {
                $this->_sMensaje = "ERROR 0001: No se pudo conectar con la base de datos \"$sNombreBD\"";
                $this->_isError = true;
                return false;
            }
           
            $isExisteBD = mysql_select_db($this->_sNombreBD, $oConnect);
            //si no se pudo encontrar esa BD lanza un error
            if(!$isExisteBD)
            {
                $this->_sMensaje = "ERROR 0002: La base de datos \"$sNombreBD\" ";
                $this->_isError = true;
                return false;
            }
            //Hay base de datos y se conecto
            else
            {
                //Guardo el id de conexión
                $this->_oLinkId = $oConnect;
                $this->_sMensaje = "Conexión realizada con la bd: \"$sNombreBD\"";  
                mysql_set_charset('utf8',$this->_oLinkId);
            }
        }
        //Ya existe recurso abierto, oLinkId!=0
        else
        {}
        return true;
    }
   
    private function conectar_sqlserver()
    {
        $sNombreBD = $this->_sNombreBD;
        if($this->_oLinkId==0 || $this->_oLinkId==null)
        {
            $oConnect = mssql_connect
            (
                $this->_sServidor,
                $this->_sUsuario,
                $this->_sClave
            );
            
            
            //bug($oConnect); die;
            if(!is_resource($oConnect))
            {
                $this->_sMensaje = "ERROR 0003: No se puede conectar a la base de datos..! ".$this->_sNombreBD;
                $this->_isError = true;
                Debug::set_vardump($this, "oDB");
                Debug::set_message($this->_sMensaje, "Database");
                return false;
            }
           
            //bug($this); die;
            $isExisteBD  = mssql_select_db($this->_sNombreBD, $oConnect);
            if(!$isExisteBD)
            {
                $this->_sMensaje = "ERROR 0004: La base de datos \"$sNombreBD\" ";
                $this->_isError = true;
                return false;
            }
            //Hay base de datos y se conecto
            else
            {
                $this->_oLinkId = $oConnect;
                $this->_sMensaje = "Conexión realizada con la bd: \"$sNombreBD\"";  
            }
        }
        //Existe recurso abierto
        else
        {}
        return true;
    }
   
    public function conectar()
    {
        $sTipoBD = $this->_sTipoBD;
        $isConnected = false;
        switch ($sTipoBD)
        {
            case "mysql":
                $isConnected = $this->conectar_mysql();
            break;
            case "sqlserver":
                $isConnected = $this->conectar_sqlserver();
            default:
            break;
        }
        return $isConnected;
    }
    //=================== FIN CONECTAR ===========================
   
    //==================== QUERY==================================
    private function query_mysql($sSQL, $sNombreTabla="table")
    {
        //Cuando se recupera un objeto desde sesion no cuenta
        //con linkid (linkid = 0)
        $oLinkId = $this->_oLinkId;
        if(empty($oLinkId))
            $this->conectar();
        try
        {
            $arTabla = array();
            $arFilas = array();
           
            //bug($this,"component_base_datos.query_mysql");
            //bug($sSQL,"component_base_datos.query_mysql");
            $oQuery = mysql_query($sSQL, $this->_oLinkId);
            //bug($oQuery,"component_base_datos.query_mysql.oquery");
            if($oQuery!=false)
            {  ////TODO error importante pq es por un fallo en la sql
                while($arFila = mysql_fetch_array($oQuery, MYSQL_ASSOC))
                {
                    $arFilas[] = $arFila;
                }
                if(!empty($sNombreTabla))
                {
                    $arTabla[$sNombreTabla] = $arFilas;
                }
                else
                {
                    $arTabla = $arFilas;
                }
            }
            else
            {
                $this->_isError = true;
                $this->_sMensaje = "ERROR EN SQL: $sSQL";
                return -1;
            }
            return $arTabla;
        }
        catch (Exception $e)
        {
            $this->_isError = true;
            $this->_sMensaje = "ERROR 0005 SQL: $sSQL, $e ";
            return -1;
        }
    }
    
    private function convert_to_utf8($arRow = array(), $origCollation = "windows-1252")
    {
        $arRowInUtf8 = array();
        foreach ($arRow as $sFieldName => $sValue)
        {
            $arRowInUtf8[$sFieldName] = iconv($origCollation, "utf-8", trim($sValue));
        }
        return $arRowInUtf8;
    }    
    
    private function convert_to_windows($arRow = array(), $origCollation = "utf-8")
    {
        $arRowInWindows = array();
        foreach ($arRow as $sFieldName => $sValue)
        {
            $arRowInWindows[$sFieldName] = iconv($origCollation, "windows-1252", $sValue);
        }
        return $arRowInWindows;
    }    
    
    
    private function query_mssql($sSQL, $sNombreTabla="table")
    {
        //Cuando se recupera un objeto desde sesion no cuenta
        //con linkid (linkid = 0)
        $oLinkId = $this->_oLinkId;
        //bug($this,"query mssql"); die;
        if(empty($oLinkId))
            $this->conectar();

        $arTabla = array();
        $arFilas = array();
       
        try
        {
            $oQuery = mssql_query($sSQL, $this->_oLinkId);
            
            if($oQuery!=false)
            {
                while($arFila = mssql_fetch_array($oQuery, MSSQL_ASSOC))
                {
                    $arFilas[] = $this->convert_to_utf8($arFila);
                }
                if(!empty($sNombreTabla))
                {
                    $arTabla[$sNombreTabla] = $arFilas;
                }
                else
                {
                    $arTabla = $arFilas;
                }
                Debug::set_sql($sSQL, mssql_num_rows($oQuery));                
                mssql_free_result($oQuery);
            }
            else
            {
                $this->_isError = true;
                $this->_sMensaje = "ERROR EN SQL: $sSQL";
                return -1;
            }            
            return $arTabla;
        }
        catch (Exception $e)
        {
            $this->_isError = true;
            $this->_sMensaje = "ERROR 0006 SQL: $sSQL, $e ";
            return -1;
        }
    }
    //=================== METODOS DE LA OTRA CLASE DE CONEXION ============
    public function query($sSQL, $sNombreTabla="")
    {
        $sTipoBD = $this->_sTipoBD;
        $arTabla = array();
        switch ($sTipoBD)
        {
            case "mysql":
                $arTabla = $this->query_mysql($sSQL,$sNombreTabla);
            break;
            case "sqlserver":
                $arTabla = $this->query_mssql($sSQL,$sNombreTabla);
            default:
            break;
        }
        
        return $arTabla;
    }
    
    public function getValue($tableName, $mixFieldName, $mixFieldValue, $sReturnField, $isStatusMobile = true) 
    {
        $sSQL = "SELECT $sReturnField FROM $tableName";

        if($mixFieldName!=null) 
        {
            $arFields=array(); 
            $arFieldsValues=array(); 
            $arConditions=array();

            if(is_array($mixFieldName))
                $arFields = $mixFieldName;
            else
                $arFields[0] = $mixFieldName;

            if(is_array($mixFieldValue))
                $arFieldsValues = $mixFieldValue;
            else
                $arFieldsValues[0] = $mixFieldValue;

            for($x=0; $x<count($arFields) || $x<count($arFieldsValues); $x++)
                array_push($arConditions, $arFields[$x]." = '".$arFieldsValues[$x]."'");

            if($isStatusMobile)
                array_push($arConditions, "status_mobile <> '99'");

            $sSQL.=" WHERE ".join(" AND ", $arConditions);
            $sSQL.=" ORDER BY $sReturnField";
            $arData = $this->query($sSQL);
            //bug($arData,$sSQL);
        }

        if(count($arData))
            return $arData[0][$sReturnField];
        else
            return false;
    }    
    //==================== FIN QUERY ================================
   
    //==================== TODO SEARCH ================================
    private static function search_mysql($sSQL)
    {
        //falta parametros en el consructor y en la función,
        //como el tipo de bd, sino se asumiria solo las constantes
        //y seria unicamente para hacer queries sobre mysql
        $oBD = CBaseDatos::get_instancia();
        $isConnected = $oBD->conectar();
        if($isConnected)
        {
            $arTabla = array();
            try
            {
                $oQuery = mysql_query($sSQL, $oBD->get_link_id());
                while($arFila = mysql_fetch_array($oQuery, MYSQL_ASSOC))
                {
                    $arTabla[] = $arFila;
                }
                return $arTabla;
            }
            catch (Exception $e)
            {
                die("ERROR 0008: No se pudo ejecutar la función search. $e");
            }            
        }
        //Error de conexion
        else
        {
            die("ERROR 0007: No se pudo conectar");
        }
    }
    private static function search_mssql($sSQL)
    {
        $oBD = CBaseDatos::get_instancia();
        $oBD->conectar();
        if($isConnected)
        {
            $arTabla = array();
            try
            {
                $oQuery = mssql_query($sSQL, $oBD->get_link_id());
                while($arFila = mssql_fetch_array($oQuery, MSSQL_ASSOC))
                {
                    $arTabla[] = $arFila;
                }
                return $arTabla;
            }
            catch (Exception $e)
            {
                die("ERROR 0010: No se pudo ejecutar la función search. $e");
            }            
        }
        //Error de conexion
        else
        {
            die("ERROR 0009: No se pudo conectar");
        }
    }
    public static function search($sSQL)
    {
        $arTabla = array();
        switch ($sTipoBD)
        {
            case "mysql":
                $arTabla = self::search_mysql($sSQL);
            break;
            case "sqlserver":
                $arTabla = self::search_mssql($sSQL);
            default:
            break;
        }
        return $arTabla;
    }
    //==================== FIN SEARCH ================================
   
    //==================== QUERY OBJECT ================================
    private function query_object_mysql($sSQL)
    {
        try
        {
            $arTabla = array();
            //TODO comprobar lo que devuelve _query
            $oQuery = mysql_query($sSQL, $this->_oLinkId);
            while($arFila = mysql_fetch_object($oQuery))
            {
                $arTabla[] = $arFila;
            }
            return $arTabla;
        }
        catch (Exception $e)
        {
            $this->_isError = true;
            $this->_sMensaje = "ERROR 0011 SQL: $sSQL, $e ";
            return -1;
        }
    }  
    
    private function query_object_mssql($sSQL)
    {
        try
        {
            $arTabla = array();
            //TODO comprobar lo que devuelve _query
            $oQuery = mssql_query($sSQL, $this->_oLinkId);
            while($arFila = mssql_fetch_object($oQuery))
            {
                $arTabla[] = $arFila;
            }
            return $arTabla;
        }
        catch (Exception $e)
        {
            $this->_isError = true;
            $this->_sMensaje = "ERROR 0012 SQL: $sSQL, $e ";
            return -1;
        }
    }  
    public function query_object($sSQL)
    {
        $sTipoBD = $this->_sTipoBD;
        $arTabla = array();
        switch ($sTipoBD)
        {
            case "mysql":
                $arTabla = $this->query_object_mysql($sSQL);
            break;
            case "sqlserver":
                $arTabla = $this->query_object_mssql($sSQL);
            default:
            break;
        }
        return $arTabla;    
    }    
    //==================== FIN QUERY OBJECT ================================
    private function execute_mysql($sSQL)
    {
                //Cuando se recupera un objeto desde sesion no cuenta
        //con linkid (linkid = 0)
        $oLinkId = $this->_oLinkId;
        if(empty($oLinkId))
            $this->conectar();

        try
        {
            //Zero if execution was successful. Non-zero if an error occurred.
            //The error code and message can be obtained by calling mysql_stmt_errno() and mysql_stmt_error().
            $isResource = mysql_query($sSQL);
            Debug::set_sql($sSQL, mysql_affected_rows());
            if(!$isResource)
            {
                $sMensaje  = "Sentencia SQL con errores: ".mysql_error()."\n";
                $sMensaje .= "SQL = $sSQL";
                $this->_isError = true;
                $this->_sMensaje = $sMensaje;
                return false;
            }
        }
        catch(Exception $e)
        {
            $sMensaje  = "Exepcion: $e \n";
            $sMensaje .= "SQL = $sSQL";
            $this->_isError = true;
            $this->_sMensaje = $sMensaje;
            return false;
        }
        $this->_isError = false;
        $this->_sMensaje = "executed: $sSQL";
        return true;
    }
   
    private function execute_mssql($sSQL)
    {
                //Cuando se recupera un objeto desde sesion no cuenta
        //con linkid (linkid = 0)
        $oLinkId = $this->_oLinkId;
        if(empty($oLinkId))
            $this->conectar();

        try
        {
            //Zero if execution was successful. Non-zero if an error occurred.
            //The error code and message can be obtained by calling mysql_stmt_errno() and mysql_stmt_error().
            $sSQL = iconv("utf-8", "windows-1252", $sSQL);
            $isResource = mssql_query($sSQL);
            if(!$isResource)
            {
                $sMensaje  = "Sentencia SQL con errores: ".mssql_error()."\n";
                $sMensaje .= "SQL = $sSQL";
                $this->_isError = true;
                $this->_sMensaje = $sMensaje;
                return false;
            }
        }
        catch(Exception $e)
        {
            $sMensaje  = "Exepcion: $e \n";
            $sMensaje .= "SQL = $sSQL";
            $this->_isError = true;
            $this->_sMensaje = $sMensaje;
            return false;
        }
        $this->_isError = false;
        $this->_sMensaje = "executed: $sSQL";
        return true;
    }  

    public function execute($sSQL)
    {
        $sTipoBD = $this->_sTipoBD;
        switch ($sTipoBD)
        {
            case "mysql":
                return $this->execute_mysql($sSQL);
            break;
            case "sqlserver":
                return $this->execute_mssql($sSQL);
            break;
            default:
            break;
        }
        Debug::set_sql($sSQL,"-");
        return null;
    }
   
    
    //GEETTTSSS==================
    private function get_servidor()
    {
        return $this->_sServidor;
    }

    public function get_usuario()
    {
        return $this->_sUsuario;
    }

    private function get_clave()
    {
        return $this->_sClave;
    }

    public function get_mensaje()
    {
        return $this->_sMensaje;
    }
   
    public function get_nombre_bd()
    {
        return $this->_sNombreBD;
    }

    private function get_link_id()
    {
        return $this->_oLinkId;
    }        
   
    public function get_tipo_bd()
    {
        return $this->_sTipoBD;
    }
}
/**/
