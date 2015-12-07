var isDebugOn = true;
function bug(variable)
{
    if(isDebugOn)
        console.debug(variable);
}

function sel_page_change(eSelect)
{
    //bug(eSelect);
    var iPage = jQuery(eSelect).find(':selected').val();
    //bug(iPage);
    jQuery('#hidNumPage').val(iPage);
    jQuery('#butSearch').click();

}


//jQuery.each http://api.jquery.com/jQuery.each/ diferencia entre recorrer un objeto y un array
//jQuery("label[for='" + this.attr("id") + "']").attr('class', 'error'); 

var oMtb =
{
    sSubmitType:"",
    arRequiredTxt:[],
    arNumeric:[],
    sRequiredEmpty:"",
    
    set_submit_type : function(sType)
                {
                    //save or delete
                    this.sSubmitType = sType;
                },    

                //TODO Mejorar estas funciones
    on_submit : function()
                {
                    //bug(oMtb.arRequiredTxt);
                    var sFieldNotRequired = '';
                    var iPosition = -1;
                    //bug("tipo submit"+this.sSubmitType);
                    if(this.sSubmitType=="save")
                    {
                        //En caso de actividades
                        sFieldNotRequired = 'det_Code_Newtype_Next';
                        //jQuery('#'+sFieldNotRequired).removeAttr('required');
                        //bug(jQuery.inArray(sFieldNotRequired,oMtb.arRequiredTxt)); return false;
                        iPosition = jQuery.inArray(sFieldNotRequired,oMtb.arRequiredTxt);
                        if(iPosition!=-1)oMtb.arRequiredTxt.splice(iPosition,1);
                        sFieldNotRequired = 'det_Date_Next';
                        iPosition = jQuery.inArray(sFieldNotRequired,oMtb.arRequiredTxt);
                        if(iPosition!=-1)oMtb.arRequiredTxt.splice(iPosition,1);
                        //bug("in save");
                        //bug(oMtb.arRequiredTxt); 
                        return this.on_form_save();
                    }
                    else if(this.sSubmitType=="delete")
                    {
                        return this.on_form_delete();
                    }
                    else if(this.sSubmitType=="replan")
                    {
                        sFieldNotRequired = 'det_Code_Newtype_Next';
                        iPosition = jQuery.inArray(sFieldNotRequired,oMtb.arRequiredTxt);
                        if(iPosition==-1) oMtb.arRequiredTxt.splice(0,0,sFieldNotRequired);
                        sFieldNotRequired = 'det_Date_Next';
                        iPosition = jQuery.inArray(sFieldNotRequired,oMtb.arRequiredTxt);
                        if(iPosition==-1) oMtb.arRequiredTxt.splice(0,0,sFieldNotRequired);
                        //bug("in replan");
                        //bug(oMtb.arRequiredTxt);   
                        return this.on_form_replan();
                    }
                    return false;
                },
                
    on_form_delete: function()
    {
        var sMensaje = "¿Está seguro de querer eliminar este registro?";
        if(confirm(sMensaje))
        {   
            jQuery("#hidAction").val("delete");
            return true;
        }
        //bug("no se envia");
        return false;
    },
    
    on_form_replan: function()
    {
        //bug("form plan");
        var sMensaje = "";
        var isRequiredOk = this.is_required_fields_ok();
        if(isRequiredOk)
        {
            sMensaje = "¿Está seguro de querer replanificar esta actividad?";
            if(confirm(sMensaje))
            {   
                jQuery("#hidAction").val("replan");
                return true;
            }
        }
        else
        {
            var sFieldId = this.sRequiredEmpty;
            var eFormControl = jQuery("#"+sFieldId);
            var eLabel = this.get_label(sFieldId);
            var sLabel = eLabel.text();
            eLabel.addClass("clsLabelError");
            eFormControl.addClass("clsTxtError");
            eFormControl.focus();
            sMensaje = "El campo "+sLabel+" es obligatorio";
            this.show_message(sMensaje);
        }        
        return false;
    },
    
    on_form_save: function()
                {
                    var sMensaje = "";
                    var isRequiredOk = this.is_required_fields_ok();
                    //bug("estado required:"+isRequiredOk);
                    if(isRequiredOk)
                    {
                        sMensaje = "¿Está seguro de guardar los cambios?";
                        if(confirm(sMensaje))
                        {
                            jQuery("#hidAction").val("save");
                            return true;
                        }
                        //No se ha aceptado continuar
                        else
                        {
                            return false;
                        }
                    }
                    //Los campos obligatorios no se han llenado completamente
                    else
                    {
                        var sFieldId = this.sRequiredEmpty;
                        var eFormControl = jQuery("#"+sFieldId);
                        var eLabel = this.get_label(sFieldId);
                        var sLabel = eLabel.text();
                        eLabel.addClass("clsLabelError");
                        eFormControl.addClass("clsTxtError");
                        eFormControl.focus();
                        sMensaje = "El campo "+sLabel+" es obligatorio";

                        this.show_message(sMensaje);
                        return false;
                    }
                },


    is_required_fields_ok: function()
                {
                    //bug("comprobando rquiered");
                    var arRequiredTxt = this.arRequiredTxt;
                    //bug(arRequiredTxt);
                    var isRequiredOk = true;
                    var isArrayOk = (arRequiredTxt!=undefined && arRequiredTxt!=null && arRequiredTxt.length >0);
                    //bug("estado array "+isArrayOk);
                    
                    if(isArrayOk)
                    {
                        //bug(arRequiredTxt);
                        jQuery.each
                        (
                            arRequiredTxt, 
                            function(iIndex,sIdText)
                            {
                                //bug(sIdText);
                                var sFieldValue = "";
                                var eFormControl = jQuery("#"+sIdText);
                                //bug(sIdText); bug(eFormControl);
                                //bug(eFormControl);
                                //existe el control
                                if(eFormControl!=null && eFormControl!=undefined && eFormControl!="" 
                                    && !oMtb.isEmpty(eFormControl))
                                {
                                    sFieldValue = eFormControl.val();
                                    sFieldValue = jQuery.trim(sFieldValue);
                                    //bug("sFieldValue: "+sFieldValue);
                                    if(sFieldValue=="" || sFieldValue==null || sFieldValue==undefined)
                                    {

                                        //bug("vacio: "+sIdText);
                                        //this es lo mismo que sIdText el item del array actual
                                        oMtb.sRequiredEmpty = sIdText;
                                        isRequiredOk = false;
                                        //bug("var isRequiredOk dentro vacio "+isRequiredOk);
                                        return false;
                                    }
                                }
                                //bug("var isRequiredOk fuera "+isRequiredOk);
                            }
                        );                    
                    }
                    return isRequiredOk;
                },//fin is_required_fields_ok

    show_message: function(sMessage)
            {
                alert(sMessage);
            },//Fin show_message
    get_label_text: function (sElementId)
            {
                return jQuery("label[for='"+sElementId+"']").text();
            },
    get_label: function(sElementId)
            {
                return jQuery("label[for='"+sElementId+"']");
            },
    show_advisor: function(sMessage,iTimeSeg)
    {
        var iTimeMilseg = 5000;
        var iTop = jQuery(window).scrollTop();
        iTop = 125;
        
        var eDivContainer = jQuery('#divPage');
        
        var sHtmlDiv = '<div id="divAdvisor" class="ui-loader ui-overlay-shadow ui-body-e ui-corner-all">';
            sHtmlDiv += '<h1>';
            sHtmlDiv += sMessage;
            sHtmlDiv += '</h1></div>';
        if(iTimeSeg!=null)
            iTimeMilseg = iTimeSeg * 1000;
            
        jQuery(sHtmlDiv)
            .css({"display": "block", "opacity": 0.96, "top": iTop})
            .appendTo(eDivContainer)
            .delay(iTimeMilseg)
            .fadeOut
            ( 
                400, function()
                {
                    jQuery(this).remove();
                }
            );
        
    },
    show_div_errormessage: function(sMessage,iTimeSeg)
    {
        var sDivId = '#divMessage';
        var iTimeSeg =  iTimeSeg || 5;
        var iTimeMiliseg = iTimeSeg * 1000;
        //alert(iTimeMiliseg);
        var oDiv = jQuery(sDivId);
        //margin-top: 10px; margin-bottom:5px; color: red; font-weight: bold; text-align:center;
        oDiv.css({"margin-top": "10px","padding":"10px","background-color":"#F4E9A8", "margin-bottom":"5px", "color":"black", "text-align":"center", "font-weight":"bold"});
        oDiv.text(sMessage);
        oDiv.show('fast');
        oDiv.delay(iTimeMiliseg).fadeOut(iTimeMiliseg);
        //delay(800).fadeIn(400)
        //bug(oDiv);
    },

    show_div_successmessage: function(sMessage,iTimeSeg)
    {
        var sDivId = '#divMessage';
        var iTimeSeg =  iTimeSeg || 5;
        var iTimeMiliseg = iTimeSeg * 1000;
        //alert(iTimeMiliseg);
        var oDiv = jQuery(sDivId);
        //margin-top: 10px; margin-bottom:5px; color: red; font-weight: bold; text-align:center;
        oDiv.css({"margin-top": "10px","padding":"10px","background-color":"#D2FFA5", "margin-bottom":"5px", "color":"#117900", "text-align":"center", "font-weight":"bold"});
        oDiv.text(sMessage);
        oDiv.show('fast');
        oDiv.delay(iTimeMiliseg).fadeOut(iTimeMiliseg);
        //delay(800).fadeIn(400)
        //bug(oDiv);
    },

    set_max_length:function(oElement,iMaxLength,event)
    {
        //http://viralpatel.net/blogs/2008/12/set-maxlength-of-textarea-input-using-jquery-javascript.html
        //alert(event.keyPressedCode);
        var keyPressed = event.which;
        //all keyPresseds including return.
        if(keyPressed >= 33 || keyPressed == 13) 
        {
            var iTextLength = oElement.value.length;
            if(iTextLength >= iMaxLength)
            {
                event.preventDefault();
            }
        }        
    },
    
    isEmpty: function (eObject) 
    {
        //alert(eObject);
        if(eObject.length != 0)
            return false;
        return true;
    }

}

jQuery(document).delegate
(
    "#frmDetail",
    "submit", 
    function() 
    {
        //this: es el elemento frmDetail
        //bug("submit ejecutado")
        return oMtb.on_submit();
    }
);


/*
function on_save()
{
    if(is_required_ok())
    {
        var sMensaje = "¿Está seguro de guardar los cambios?";
        if(confirm(sMensaje))
        {

            jQuery("#hidAction").val("save");
            return true;
        }
    }
    return false;
}

function on_delete()
{
}
                */
    

                


//NIU
//En jqmobile no se puede tratar el evento ready como en páginas comunes.
//el evento equivalente es pagecreate del div data-role
/*
jQuery("div[data-role*='page']").live
(   
    "pagecreate", 
    function(event)
    {
        on_data_role_pagecreate()
    }
);*/
