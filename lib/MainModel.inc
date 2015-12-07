<?php
//Model configuration
class MainModel 
{
    /**
     * @var Database
     */    
    protected $oDB = null;

    //Hydra
    protected $_table_name = "";
    protected $_table_code = "";
    //Nombres de los campos tipo numerico
    protected $_arNumericFields = array();
    //Nombres de los campos de tipo string
    //protected $_arStringFields = array(); este sobra se supone que si no es numerico es tipo string
    //Nombre de los campos que son primary
    protected $_arPrimKeysFields = array();
    //Para Insercion manual y actualizacion segun claves.
    protected $_arPrimKeysAndValues = array();
    protected $_useAutoKeys = false;
    protected $_isAutoKeyNumeric = false;
    
    protected $_arTable = array();
    protected $_message = array();
    protected $_arData = array();
    
    protected $_code_user_session = "";
    
    protected $_default_sql;
    //Código generico para todos los modelos
    protected $_code; 
    
    protected $_list_url = "";
    protected $_list_items_per_page = 30;
    protected $_list_page = 0;
    protected $_list_num_pages = 0;
    protected $_list_arPages = array();
    protected $_list_total_regs = 0;
    
    public function __construct()
    {
        global $oDB; //No site pq aplica recursividad
        $this->oDB = $oDB;
    }
    
    public function get_random_value($isNumeric=false, $iLength=12)
    {
        $sRandom = "";
        $sChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        $sNumbers = "0123456789";
        
        if($isNumeric)
            $sChars = $sNumbers;
        else
            $sChars = $sNumbers . $sChars;
        
        if($iLength>0)
        {
            $sChars = str_split($sChars,1);
            //bug($sChars,"schars"); die;
            for($i=1; $i<=$iLength; $i++)
            {
                mt_srand((double)microtime() * 1000000);
                $iPosition = mt_rand(1,count($sChars));
                $sRandom .= $sChars[$iPosition-1];
            }
	}        

        return $sRandom;
    }   

    public function is_value_in_field($snValue,$sFieldName="Code",$isNumeric=false,$sTableName="")
    {
        if(empty($sTableName))
            $sTableName = $this->_table_name;
        
        if(!$isNumeric)
            $snValue = $this->get_as_string_value($snValue);
        
        $sSQL = "SELECT COUNT($sFieldName) AS result FROM $sTableName WHERE $sFieldName=$snValue ";
        
        //bug($sSQL,"IS VALUE IN FIELD"); 
        $arResult = $this->oDB->query($sSQL);
        $iResult = (int)$arResult[0]["result"];
        //bug($iResult,"iResult is_valu_in_field"); die;
        if($iResult>0)
            return true;
        return false;
    }
    
    public function get_autokey($sFieldName,$isNumbersOnly=false,$iLength=12,$sTableName="")
    {
        $iMaxTry = 3; $snValue = "err".time();
        if(empty($sTableName))
            $sTableName = $this->_table_name;
        if(!empty($sFieldName))
        {    
            $isInField = true;  $iTry = 0; 
            while($isInField && $iTry<$iMaxTry) 
            {
                $snValue = $this->get_random_value($isNumbersOnly, $iLength);
                //Comprueba si ya existe este valor aleatorio
                $isInField = $this->is_value_in_field($snValue,$sFieldName, $isNumbersOnly, $sTableName);
                $iTry++;
            }
        }
       
        return $snValue;
    }
    
    public function get_max_numeric($sFieldName="Code")
    {
        $sSQL = "SELECT MAX(CONVERT(INT,$sFieldName)) AS max
                FROM $this->_table_name
                ";
        //bug($sSQL); die;
        $arResult = $this->oDB->query($sSQL);
        $iMax = (int)$arResult[0]["max"];
        return $iMax;
    }
    
    public function get_max_numeric_plus($sFieldName="Code")
    {
        $iMax = (int)$this->get_max_numeric($sFieldName);
        $iMax++;
        return $iMax;
    }
    
    public function build_sqlin($arValues=array(),$isNumeric=false)
    {
        ($isNumeric)? $sSQLIn="(-1)" : $sSQLIn = "('')";
        $arString = array();
        if(!empty($arValues))
        {
            if(!$isNumeric)
            {
                foreach($arValues as $sValue)
                {    $arString[] = "'$sValue'"; }
                $arValues = $arString;
            }
            
            $sValues = join(",",$arValues);
            $sSQLIn = "($sValues)";
        }
        return $sSQLIn;
    }
    
    private function get_as_string_value($sFieldValue)
    {
        return "'$sFieldValue'";
    }
    
    private function get_value_by_type($sFieldName,$sFieldValue)
    {
        //Si no es numerico
        if(!in_array($sFieldName, $this->_arNumericFields))
        {
            return $this->get_as_string_value($sFieldValue);
        }
        elseif(empty($sFieldValue))
        {
            //Si es numerico y está vacio
            $sFieldValue = 0;
        }
        return $sFieldValue;
    }
    
    protected function build_sql_delete($arFieldAndValue)
    {
        $sSQLDelete = "DELETE FROM $this->_table_name ";
        $sSQLDelete .= "WHERE 1=1 ";
        $sSQLCondition = $this->build_sql_condition($arFieldAndValue);
        return $sSQLDelete . $sSQLCondition;
    }
    
    /**
     * Se usa $this->_arPrimKeysAndValues si autokeys esta en off
     * de lo contrario se autogeneran las claves y se cargan sus nombres y 
     * valores en esta variable para posteriormente poderla usar en un modelo
     * extendido
     * @param array $arData
     * @return string Sentecia INSERT 
     */
    protected function build_sql_insert($arData=array())
    {
        $sSQLInsert = "INSERT INTO $this->_table_name \n";
        
        if($this->_useAutoKeys)
        {
            foreach($this->_arPrimKeysFields as $sFieldName)
            {
                $arFieldName[] = $sFieldName;
                $sAutoKey = $this->get_autokey($sFieldName);
                $arFieldValue[] = $this->get_value_by_type($sFieldName, $sAutoKey);;
                //lleno este array para poder recuperar las claves generadas y de ese modo
                //hacer la inserción del modelo extendido
                $this->_arPrimKeysAndValues[$sFieldName] = $sAutoKey;
            }
        }
        //Inserción manual
        else
        {
            foreach($this->_arPrimKeysAndValues as $sFieldName=>$mxKeyValue)
            {
                $arFieldName[] = $sFieldName;
                $arFieldValue[] = $this->get_value_by_type($sFieldName, $mxKeyValue);
            }
        }
        
        foreach($arData as $sFieldName=>$mxValue)
        {
            $arFieldName[] = $sFieldName;
            $arFieldValue[] = $this->get_value_by_type($sFieldName, $mxValue);
        }
        $sFieldsNames = join(",", $arFieldName);
        $sFieldsNames = "($sFieldsNames)\n";
        $sFieldsValues = join(",",$arFieldValue);
        $sFieldsValues = "($sFieldsValues)\n";
        
        $sSQLInsert .= $sFieldsNames . "VALUES\n";
        $sSQLInsert .= $sFieldsValues;
        
        //bug($sSQLInsert);
        return $sSQLInsert;
    }
    
    protected function build_sql_update
    ($arData=array(),$arCondFields=array(),$arPrimKeys=array())
    {
        if(empty($arPrimKeys))
            $arPrimKeys = $this->get_field_names($this->_arPrimKeysAndValues);
        //bug($arData); bug($arPrimKeys); die;
        $sSQLUpdate = "UPDATE $this->_table_name SET ";
        foreach($arData as $sFieldName=>$mxValue)
        {
            //Crea la sentencia set de los campos que no son Primary Keys
            if(!in_array($sFieldName, $arPrimKeys))
            {
                $sValue = $this->get_value_by_type($sFieldName, $mxValue);
                $arSet[] = "$sFieldName=$sValue";
            }
        }
        $sSQLSet = join(",",$arSet);
        $sSQLUpdate .= $sSQLSet;
        $sSQLUpdate .= " WHERE 1=1 ";
        $sSQLAnd = $this->build_sql_condition($arCondFields);
        $sSQLUpdate .= $sSQLAnd;
        return $sSQLUpdate;        
    }
    
    /**
     * $arFieldsAndValues del tipo array("FieldName1"=>"ValueField1",...)
     * @param array $arFieldsAndValues
     * @return array tipo array("ValueField1","ValueField2"...)
     */
    protected function get_field_names($arFieldsAndValues=array())
    {
        return array_keys($arFieldsAndValues);
    }

    /**
     * 
     * @param array $arRows Tipo array(array("fieldName1"=>"fieldValue1",..,"fieldNameN"=>"fieldValueN"),array(..))
     * @param string $sFieldName Nombre de la la columna de la que se quiere obtener sus datos
     * @return array array(fieldValue1,...,fieldValueN)
     */
    protected function get_field_values($arRows, $sFieldName)
    {
        $arValues=array();
        if(!empty($arRows))
            foreach($arRows as $arFields)
            {
                foreach($arFields as $sFldName => $sValue)
                {
                    if($sFldName==$sFieldName)
                        $arValues[] = $sValue;
                }
            }
        return $arValues;
    }
    
    /**
     * Antes de llamar a este metodo hay que configurar el modelo
     * Si se va a usar autokeys indicarlo con set_autokeys_on() 
     * cargar los nombres de campos numericos set_numeric_fields()
     * cargar los nombres de los campos que son clave primaria con set_primarykeys_names()
     * @param array $arData Array recuperado desde Post
     */
    public function auto_insert($arData=array())
    {
        if(empty($arData))
            $arData = $this->_arData;
        $sSQL = $this->build_sql_insert($arData);
        //bug($sSQL,"AUTO INSERT"); die;
        $this->oDB->execute($sSQL);
    }
    
    public function auto_update($arData=array(),$arCond=array(),$arPrimKeys=array())
    {
        if(empty($arData))
            $arData = $this->_arData;
        $sSQL = $this->build_sql_update($arData, $arCond, $arPrimKeys);
        //bug($sSQL); die;
        $this->oDB->execute($sSQL);
    }
    
    public function auto_delete($arKeysValues=array())
    {
        if(empty($arKeysValues))
            $arKeysValues = $this->_arPrimKeysAndValues;
            
        $sSQL = $this->build_sql_delete($arKeysValues);
        bug($sSQL); 
        $this->oDB->execute($sSQL);
    }
    /**
     * @param array $arFieldAndValue Campos y valores para construir la consulta like
     * @param array $arMultOr Nombres de campos que se construiran con OR
     * @return string 
     */
    protected function build_sql_like_for_search($arFieldAndValue,$arMultOr=array())
    {
        $sConditionLike = "";
        $arSQLOr = array();
        $arSQLAnd = array();

        if(!empty($arFieldAndValue))
        {
            foreach($arFieldAndValue as $sFieldName => $sValue)
            {
                if($sValue!="" && !in_array($sFieldName, $arMultOr))
                    $arSQLAnd[] = "$sFieldName LIKE '%$sValue%'";
            }
            //Multiple like 
            if(!empty($arMultOr))
            {   
                $sSQLOr = "";
                foreach($arFieldAndValue as $sFieldName => $sValue)
                {
                    if($sValue!="" && in_array($sFieldName, $arMultOr))
                        $arSQLOr[] = "$sFieldName LIKE '%$sValue%'";
                }            
                if(!empty($arSQLOr))
                {    
                    $sSQLOr = join(" OR ",$arSQLOr);
                    $sSQLOr = "($sSQLOr)";
                }
                if(!empty($sSQLOr))
                    $arSQLAnd[] = $sSQLOr;
            }

            if(!empty($arSQLAnd))
                $sConditionLike = join(" AND ",$arSQLAnd)."\n";
        }
        return $sConditionLike;
    }
    
    protected function build_sql_groupby($arFieldNames=array())
    {
        $sGroupBy = join(",",$arFieldNames);
        if(!empty($sGroupBy))
            $sGroupBy = "GROUP BY $sGroupBy\n";
        else
            $sGroupBy = "";
        return $sGroupBy;
    }
    
    protected function build_sql_orderby($arFieldNames=array(),$sType="ASC")
    {
        $sOrderBy = join(",",$arFieldNames);
        if(!empty($sOrderBy))
            $sOrderBy = "ORDER BY $sOrderBy $sType\n";
        else
            $sOrderBy = "";
        return $sOrderBy;
    }    
    
    protected function build_sql_concatid($arFieldNames=array(),$sFieldName="AutoId")
    {
        $sIdConcat = $sFieldName;
        $arConcat = array();
        foreach($arFieldNames as $sFieldName)
            $arConcat[] = "LTRIM(RTRIM($sFieldName))";
        if(!empty($arFieldNames)) $sIdConcat = $sIdConcat."=".join("+'-'+",$arConcat);
        return $sIdConcat;
    }
    
    //Solo strings
    protected function build_sql_condition($arFieldAndValue, $sGlue="AND")
    {
        $arSqlCondition = array(); $sSQLCondition = "";
        if(!empty($arFieldAndValue))
        {
            foreach($arFieldAndValue as $sFieldName => $sFieldValue)
            {
                $sValue = $this->get_value_by_type($sFieldName, $sFieldValue);
                $arSqlCondition[] = "$sFieldName=$sValue";
            }
            $sSQLCondition = " AND ".join(" $sGlue ",$arSqlCondition);
        }
        return $sSQLCondition;
    }
    
    protected function get_fieldvalue_exploded($sFieldName, $sMark=",")
    {
        $arKeysExploded = explode($sMark,$sFieldName);
        return $arKeysExploded;
    }
    
    //To overwrite. used in load_data_by_keys
    //protected function set_all_properties($arData){}
    
    /**
     * Antes de llamar este metodo se ha de crear prot load_in_properties($arData){}
     * @param string $sSQL
     */
    protected function load_from_sql($sSQL)
    {
        $arData = $this->oDB->query($sSQL);
        $arData = $arData[0];
        //bug($arData); 
        //override this method
        $this->set_all_properties($arData);        
    }

    public function load_by_code()
    {
        $sSQL = $this->_default_sql;
        $sSQL .= " WHERE Code='$this->_code'";
        //bug($sSQL,"en load by code");
        $this->load_from_sql($sSQL);
    }
    
    public function set_numeric_fields($arArray=array())
    {
        $this->_arNumericFields = $arArray;
    }
    
    /**
     * @param array $arKeysAndValues  fieldname=>value
     */
    public function load_data_by_keys($arKeysAndValues=array())
    {
        if(!empty($arKeysAndValues))
        {
            $sSQLAnd = $this->build_sql_condition($arKeysAndValues);
            $sSQL = "SELECT * FROM $this->_table_name WHERE 1=1 $sSQLAnd";
            $this->load_from_sql($sSQL);
        }
    }
    
    public function load_table()
    {
        $sSQL = "SELECT *
                 FROM $this->_table_name";
        $this->_arTable = $this->oDB->query($sSQL);        
    }

    public static function get_data_for_picklist($sTable,$Code="Code",$Description="Description", $sSQLAnd="", $sOrderBy="", $sOrder="",$useBlankItem=true)
    {
        global $oDB;
        if(empty($Code)) $Code="Code";
        if(empty($Description)) $Description="Description";
        
        $sSQL="SELECT DISTINCT $Code, $Description FROM $sTable WHERE 1=1 ";
        if(!empty($sSQLAnd)) $sSQL .= "AND $sSQLAnd ";
        if(empty($sOrderBy)) $sOrderBy = $Description;
        if(empty($sOrder)) $sOrder = "ASC";
        
        $sSQL.=" ORDER BY $sOrderBy $sOrder";
        //if($sTable=="MTB_Productos") bug($sSQL,"pick sql",1);
        $arRows = $oDB->query($sSQL);
        //if($sTable=="MTB_Products_SubFamily") bug($arRows,$sSQL); 
        
        if($useBlankItem) $arPickList = array(""=>"");
        
        //ERROR PARA SUBFAMILIAS POR ESTO: MISMO CODIGO PARA DISTINTAS SUBFAMILIAS
        /*02	ACC. DE LATÓN
          02	ACC. DE LATÓN CON BRIDAS 75-90-110
          02	ACC. GALVANIZADO
         */
        foreach($arRows as $i=>$arFields)  
            $arPickList[$arFields[$Code]]=$arFields[$Description];
        
        /*
        $iNumRows = count($arRows);
        for($i=0; $i<$iNumRows; $i++)
        {
            $arRow = $arRows[$i];
            //if($sTable=="MTB_Products_SubFamily") bug($arRow);
            $sValue =(string)$arRow[$Code];
            $sText = (string)$arRow[$Description];
            $arTemp = array($sValue=>$sText);
            $arPickList = array_merge($arPickList,$arTemp);
            //$arPickList[$sValue] = $sText;
            if($sTable=="MTB_Products_SubFamily") bug($arPickList,"$sValue=$sText");
        }*/
        //if($sTable=="MTB_Products_SubFamily") bug($arPickList,"piclist a dev",1);
        return $arPickList;
    }
    
    public function get_table()
    {
        return $this->_arTable;
    }    

    public function get_table_name()
    {
        return $this->_table_name;
    }
    
    public function get_table_code()
    {
        return $this->_table_code;
    }
    
    public function set_list_url($sUrl)
    {
        $this->_list_url = $sUrl;
    }

    public function set_list_items_per_page($iItems)
    {
        $this->_list_items_per_page = $iItems;
    }
    
    public function set_list_page($iPage=1)
    {
        $this->_list_page = $iPage;
    }
    
    public function is_error()
    {
        if(!empty($this->_message))
        {
            return true;
        }
        return false;
    }
    
    public function get_message()
    {
        return $this->_message;
    }
    
    public function get_list_num_pages()
    {
        return $this->_list_num_pages;
    }
    
    /**
     * @return array Para select de páginado
     */
    public function get_list_pages()
    {
        return $this->_list_arPages;
    }
    
    public function get_total_regs()
    {
        return $this->_list_total_regs;
    }
    
    public function set_autokeys_on($isOn=true)
    {
        $this->_useAutoKeys = $isOn;
    }
    
    public function set_autokeys_numeric_on($isOn=true)
    {
        $this->_isAutoKeyNumeric = $isOn;
    }
    
    /**
     * Obligatoriamente se debe configurar las claves y sus valores si se utiliza autokeys=off
     * si se desea utilizar autoinsert
     * @param array $arFieldsAndValues array("primKey1"=>"valor1",..."primKeyn"=>"valorn")
     */
    public function set_keys_and_values($arFieldsAndValues)
    {
        $this->_arPrimKeysAndValues = $arFieldsAndValues;
    }
    
    public function get_keys_and_values()
    {
        return $this->_arPrimKeysAndValues;
    }
    
    public function get_list_page()
    {
        return $this->_list_page;
    }
    
    /**
     * Nombres de los campos que son claves primarias.
     * Insert: Se utilizan para cargar arPrimKeysAndValues de modo que se pueda
     * reutilizar en un modelo extendido.
     * @param array $arPrimaryKeys 
     */
    public function set_primarykeys_names($arPrimaryKeys)
    {
        $this->_arPrimKeysFields = $arPrimaryKeys;
    }
}