<?php
class ComponentText extends MainComponent
{
    public static function clean_before_save_db(&$arData)
    {
        array_walk_recursive($arData, "ComponentText::clean_array_text_values");
    }
    
    public static function clean_array_text_values(&$mxValue, $key)
    {
        if (!is_array($mxValue))
        {
            //bug($mxValue, "antes de limpiar");
            $mxValue = self::clean_for_db($mxValue);
            //bug($mxValue, "despues de limpiar");
        }
    } 
    
    private static function clean_for_db($sText)
    {
        //Si ya viene escapadas las comilas quito las barras de escape
        //para que quede en texto plano
        if(get_magic_quotes_gpc())
        {
            $sText = str_replace("\'", "'", $sText);
            $sText = str_replace("\\\"", "\"", $sText);
        }
            //$sText = stripslashes ($sText);
        //Pasa los caracteres de entidades html conflictivos:
        //comillas, mayor, menor y barra invertida
        //a texto simple
        $sText = self::htmlentities_to_plain_text($sText);
        //se escapan las comillas, solo las simples
        //$sText = addslashes($sText); en mssql no se escapa con \
        $sText = str_replace("'", "''", $sText);
        //se escapan las barras invertidas
        //$sText = str_replace("\\", "\\\\", $sText);
        return $sText;
    }
    
    private static function htmlentities_to_plain_text($sText)
    {
        //	&#92; \
        $sText = str_replace("&#039;", "'", $sText);
        $sText = str_replace("&gt;", ">", $sText);
        $sText = str_replace("&quot;", "\"", $sText);   
        $sText = str_replace("&lt;", "<", $sText);
        $sText = str_replace("&#92;", "\\", $sText);
        return $sText;
    }
    
    public static function clean_for_html($sPlainText)
    {
        //bug($sDbText); en mssql no se guarda con slashes como en mysql en lugar de slashes se
        //utiliza ' . El sqlinjection solo se podria aplicar con este tipo de comillas no con las dobles.
        //$sHtmlText = stripslashes($sDbText);
        $sHtmlText = str_replace("<", "&lt;", $sPlainText);
        $sHtmlText = str_replace(">", "&gt;", $sHtmlText);
        $sHtmlText = str_replace("\"", "&quot;", $sHtmlText);
        $sHtmlText = str_replace("'", "&#039;", $sHtmlText);
        //esto evitaria que se cree un html como value="absbdf\" con lo cual se escaparia el último "
        $sHtmlText = str_replace("\\", "&#92;", $sHtmlText);
        return $sHtmlText;
    }
}