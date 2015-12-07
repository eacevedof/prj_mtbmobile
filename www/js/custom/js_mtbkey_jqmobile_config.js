jQuery(document).bind
(
    "mobileinit", 
    function()
    {
        jQuery.extend
        (  
            jQuery.mobile, 
            {
                ajaxEnabled: false
            }
        );
    }
);

/*
$(document).bind
(   "mobileinit", 
    function()
    {
        alert("hola mundo");
         jQuery.mobile.ajaxEnabled= false;
        /*
	$.extend
        (  
            $.mobile , 
            {
		loadingMessage: 'Cargando',
		pageLoadErrorMessage: 'Error al cargar',
		defaultPageTransition: 'fade'
            }
        );
        *
    }
);*/