//Este script debe ir despues de cargar el jqmobile
jQuery.extend
(
    jQuery.mobile.datebox.prototype.options.lang, 
    {
        'es-ES': 
        {
            setDateButtonLabel: 'Guardar Fecha',
            setTimeButtonLabel: 'Guardar Hora',
            setDurationButtonLabel: 'Guardar Duraciónn',
            calTodayButtonLabel: 'Hoy',
            titleDateDialogLabel: 'Elegir Fecha',
            titleTimeDialogLabel: 'Elegir Hora',
            daysOfWeek: ['Domingo','Lunes','Martes','Miercoles','Jueves','Viernes','Sabado'],
            daysOfWeekShort: ['Do','Lu','Ma','Mi','Ju','Vi','Sa'],
            monthsOfYear: ['Enero','Febrero','Marzo','Abril','Mayo','Junio','Julio','Agosto','Septiembre','Octubre','Noviemebre','Diciembre'],
            monthsOfYearShort: ['Ene','Feb','Mar','Abr','May','Jun','Jul','Ago','Sep','Oct','Nov','Dic'],
            durationLabel: ['Dias','Horas','Minutos','Segundos'],
            durationDays: ['Dia','Dias'],
            timeFormat: 24,
            dateFieldOrder: ['d', 'm', 'y'],
            timeFieldOrder: ['h', 'i', 'a'],
            slideFieldOrder: ['d', 'm', 'y'],
            headerFormat: 'ddd, dd mmm , YYYY',
            dateFormat: 'DD/MM/YYYY',
            useArabicIndic: false,
            clearButton: 'limpiar',
            isRTL: false
        }
    }
);
jQuery.extend
(
    jQuery.mobile.datebox.prototype.options, 
    {
        useLang: 'es-ES',
        mode: 'calbox',
        calStartDay: 1,
        //useClearButton: true, se activa y desactiva en el propio control
        disableManualInput: true,
        noButtonFocusMode: true,
        pickPageButtonTheme: 'd',
        pickPageHighButtonTheme: 'b'
        /**/
    }
);


    
