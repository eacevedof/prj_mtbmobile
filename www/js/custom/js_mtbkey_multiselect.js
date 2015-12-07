function on_sch_mes_change(eSelect)
{
    var sElementId = jQuery(eSelect).attr('id');
    var arOptSelected = jQuery('#'+sElementId+' option:selected');
    var isTodosSelected = false;
    //bug(arOptSelected);
    jQuery.each
    (   
        arOptSelected, 
        function(iPositon, eOption) 
        { 
            if('todos'==jQuery(eOption).val())
            {
                isTodosSelected = true;
                //equivalente al break. si se devuelve true
                return false;
            }
                
        }
    );
    if(isTodosSelected)//se deselecciona el resto
    jQuery.each(arOptSelected, function(iPosition, eOption) 
        { 
            if('todos'!=jQuery(eOption).val())
                jQuery(eOption).attr('selected',false);
        }
    );
    //Actualiza los elementos html asociados al control, como divs, span etc...
    jQuery(eSelect).selectmenu("refresh",true);
}