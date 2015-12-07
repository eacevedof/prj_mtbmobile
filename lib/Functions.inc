<?php
/*
 * Created on 18/01/2012 by Pedro Alonso
 */
function bug($var, $sNombreVariable='var', $isDie=false )
{
    $sTagPre = "<pre style=\"background:#CDE552; padding:0px; color:black; font-size:12px; \" >";
    $sTagFinPre = "</pre>";
    $nombreVariable = $sTagPre . 'VARIABLE <b>'.$sNombreVariable.'</b>:';
    $nombreVariable .= $sTagFinPre;
    echo $nombreVariable;
    echo  "<pre style=\" background:#E2EDA8; font-size:12px; padding-left:10px; text-align:left; color:black; font-weight:normal; font-family: \'Courier New\', Courier, monospace !important; \" >";
    var_dump($var);
    echo  "</pre>";

    if($isDie)
    {
       die;  
    }
}

function bugfile($sFilePath, $sNombreVariable='var', $isDie=false )
{
    bug(is_file($sFilePath),$sNombreVariable,$isDie);
}

function bugdir($sDirPath, $sNombreVariable='var', $isDie=false )
{
    bug(is_dir($sDirPath),$sNombreVariable,$isDie);
}

function file_exists_in_incpath($sFileName)
{
    $arIncludedPaths = explode(";",get_include_path());
    foreach($arIncludedPaths as $sDirPath)
    {
        $sFilePath = $sDirPath . $sFileName;
        $isInDir = is_file($sFilePath);
        if($isInDir)
            return true;
    }
    return false;
}

function to_user_date($sHydraDate)
{
    $sUserDate = "";
    if(!empty($sHydraDate))
    {
        //bug($sHydraDate); die; 2001 10 25
        $sYear = substr($sHydraDate,0,4);
        $sMonth = substr($sHydraDate,4,2);
        $sDay = substr($sHydraDate,6,2);
        $sUserDate = "$sDay/$sMonth/$sYear";
        //bug($sHydraDate."-".$sUserDate);
    }
    return $sUserDate;
}

function to_hydra_date($ddmmyy)
{
    
}