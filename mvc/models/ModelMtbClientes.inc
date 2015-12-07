<?php
require_once "ModelClients.inc";
class ModelMtbClientes extends MainModel implements InterfaceModel
{
    protected $_code;    
    private $_baja_logica;
    private $_cod_baja;
    private $_cod_banco;
    private $_create_date;
    private $_create_user;
    private $_credito_caution;
    private $_dc;
    private $_delete_date; //2011 09 28 13 38 44
    private $_delete_user;
    private $_descuento;
    private $_descuento2;
    private $_desea_informacion;
    private $_dto_prontopago;
    private $_empresa;
    private $_enviado_a_erp;
    private $_estado;
    private $_fecha_alta;
    private $_fecha_baja;
    private $_forma_pago;
    private $_iban;
    private $_id;
    private $_modify_date;
    private $_modify_user;
    private $_num_cuenta;
    private $_numero_calle;
    private $_observaciones;
    private $_poblacion;
    private $_sucursal;
    private $_tipo_via;
    private $_ultima_operacion;
    private $_validado;
    
    public $oClient = null;
    
    public function __construct(ModelClients $oClient=null,
        $code="", $baja_logica="", $cod_baja="", $cod_banco="", $create_date="",
        $create_user="", $credito_caution="", $dc="", $delete_date="", $delete_user="",
        $descuento="", $descuento2="", $desea_informacion="", $dto_prontopago="",
        $empresa="", $enviado_a_erp="", $estado="", $fecha_alta="", $fecha_baja="",
        $forma_pago="", $iban="", $id="", $modify_date="", $modify_user="", $num_cuenta="",
        $numero_calle="", $observaciones="", $poblacion="", $sucursal="", $tipo_via="",
        $ultima_operacion="", $validado="")
    {
        global  $oDB; 
        $this->oDB = $oDB;
        $this->_table_name = "MTB_Clientes";
        $this->_table_code = "326";

        $sSQL = "SELECT
                    Id, Create_User, Create_Date, Modify_User, Modify_Date, Delete_User, Delete_Date, Code, Numero_Calle, Poblacion, Cod_Banco, Sucursal, DC, Num_Cuenta, 
                    Credito_Caution, Descuento, Descuento2, Dto_ProntoPago, Observaciones, Estado, Fecha_Alta, Tipo_Via, Forma_Pago, Desea_Informacion, Validado, Fecha_baja, 
                    Cod_Baja, Baja_logica, Empresa, Enviado_a_Erp, Iban, Ultima_Operacion 
                 FROM MTB_Clientes";
        $this->_default_sql = $sSQL;
        
        $this->_code = $code;
        $this->oClient = new ModelClients(null, $code);
        if(!empty($oClient))
        {
            $this->oClient = $oClient;
            $this->_code = $oClient->get_code();
        }
        
        $this->_baja_logica = $baja_logica;
        $this->_cod_baja = $cod_baja;
        $this->_cod_banco = $cod_banco;

        $this->_create_date = $create_date;
        $this->_create_user = $create_user;
        $this->_credito_caution = $credito_caution;
        $this->_dc = $dc;
        $this->_delete_date = $delete_date;
        $this->_delete_user = $delete_user;
        $this->_descuento = $descuento;
        $this->_descuento2 = $descuento2;
        $this->_desea_informacion = $desea_informacion;
        $this->_dto_prontopago = $dto_prontopago;
        $this->_empresa = $empresa;
        $this->_enviado_a_erp = $enviado_a_erp;
        $this->_estado = $estado;
        $this->_fecha_alta = $fecha_alta;
        $this->_fecha_baja = $fecha_baja;
        $this->_forma_pago = $forma_pago;
        $this->_iban = $iban;
        $this->_id = $id;
        $this->_modify_date = $modify_date;
        $this->_modify_user = $modify_user;
        $this->_num_cuenta = $num_cuenta;
        $this->_numero_calle = $numero_calle;
        $this->_observaciones = $observaciones;
        $this->_poblacion = $poblacion;
        $this->_sucursal = $sucursal;
        $this->_tipo_via = $tipo_via;
        $this->_ultima_operacion = $ultima_operacion;
        $this->_validado = $validado;
    }


    
    protected function set_all_properties($arData) 
    {
        $this->_cod_baja = $arData["Cod_Baja"];
        $this->_cod_banco = $arData["Cod_Banco"];
        $this->_code = $arData["Code"];
        $this->_create_date = $arData["Create_Date"];
        $this->_create_user = $arData["Create_User"];
        $this->_credito_caution = $arData["Credito_Caution"];
        $this->_dc = $arData["DC"];
        $this->_delete_date = $arData["Delete_Date"];
        $this->_delete_user = $arData["Delete_User"];
        $this->_descuento = $arData["Descuento"];
        $this->_descuento2 = $arData["Descuento2"];
        $this->_desea_informacion = $arData["Desea_Informacion"];
        $this->_dto_prontopago = $arData["Dto_ProntoPago"];
        $this->_empresa = $arData["Empresa"];
        $this->_enviado_a_erp = $arData["Enviado_a_Erp"];
        $this->_estado = $arData["Estado"];
        $this->_fecha_alta = $arData["Fecha_Alta"];
        $this->_fecha_baja = $arData["Fecha_baja"];
        $this->_forma_pago = $arData["Forma_Pago"];
        $this->_iban = $arData["Iban"];
        $this->_id = $arData["Id"];
        $this->_modify_date = $arData["Modify_Date"];
        $this->_modify_user = $arData["Modify_User"];
        $this->_num_cuenta = $arData["Num_Cuenta"];
        $this->_numero_calle = $arData["Numero_Calle"];
        $this->_observaciones = $arData["Observaciones"];
        $this->_poblacion = $arData["Poblacion"];
        $this->_sucursal = $arData["Sucursal"];
        $this->_tipo_via = $arData["Tipo_Via"];
        $this->_ultima_operacion = $arData["Ultima_Operacion"];
        $this->_validado = $arData["Validado"];     
    }

    public function delete(){}
    public function save(){}

    //No se utiliza pq se usa array de insercion
    public function update_after_new()
    {
        $sSQL="UPDATE MTB_Clientes 
             SET Validado='0',
             Ultima_Operacion='A'
             WHERE Ultima_Operacion IS NULL";
        $this->oDB->execute($sSQL);
    }
    
    //No se utiliza pq se usa el array de modificaciónn
    public function update_after_edit()
    {
        $sCode = $this->_code;
        $sSQL="UPDATE MTB_Clientes 
             SET Validado='0',
             Ultima_Operacion='M',
             Enviado_a_Erp='0'
             WHERE Code='$sCode'";
        $this->oDB->execute($sSQL);        
    }
    
    public function get_left_table()
    {
        $sSQL = "SELECT clients.Code, clients.Codigo_Sic, clients.Correo_E,
                clients.Cp, clients.Direccion, clients.Fax, clients.Name,
                clients.Pais, clients.Propietario, clients.Provincia,
                clients.Tlf2, MTB_Clientes.Baja_logica, MTB_Clientes.Cod_Baja,
                MTB_Clientes.Cod_Banco, MTB_Clientes.Code, MTB_Clientes.Create_Date,
                MTB_Clientes.Create_User, MTB_Clientes.Credito_Caution, MTB_Clientes.DC,
                MTB_Clientes.Delete_Date, MTB_Clientes.Delete_User, MTB_Clientes.Descuento,
                MTB_Clientes.Descuento2, MTB_Clientes.Desea_Informacion, MTB_Clientes.Dto_ProntoPago,
                MTB_Clientes.Empresa, MTB_Clientes.Enviado_a_Erp, MTB_Clientes.Estado,
                MTB_Clientes.Fecha_Alta, MTB_Clientes.Fecha_baja, MTB_Clientes.Forma_Pago,
                MTB_Clientes.Iban, MTB_Clientes.Id, MTB_Clientes.Modify_Date,
                MTB_Clientes.Modify_User, MTB_Clientes.Num_Cuenta, MTB_Clientes.Numero_Calle,
                MTB_Clientes.Observaciones, MTB_Clientes.Poblacion, MTB_Clientes.Sucursal,
                MTB_Clientes.Tipo_Via, MTB_Clientes.Ultima_Operacion,MTB_Clientes.Validado
                FROM clients
                LEFT JOIN MTB_Clientes
                ON clients.Code
                WHERE clients.Delete_Date is null
                AND 1=1
                ";
        $arData = $this->oDB->query($sSQL);
    }
    
    public function get_baja_logica(){ return $this->_baja_logica; }
    public function get_cod_baja(){ return $this->_cod_baja; }
    public function get_cod_banco(){ return $this->_cod_banco; }
    public function get_code(){ return $this->_code; }
    public function get_create_date(){ return $this->_create_date; }
    public function get_create_user(){ return $this->_create_user; }
    public function get_credito_caution(){ return $this->_credito_caution; }
    public function get_dc(){ return $this->_dc; }
    public function get_delete_date(){ return $this->_delete_date; }
    public function get_delete_user(){ return $this->_delete_user; }
    public function get_descuento(){ return $this->_descuento; }
    public function get_descuento2(){ return $this->_descuento2; }
    public function get_desea_informacion(){ return $this->_desea_informacion; }
    public function get_dto_prontopago(){ return $this->_dto_prontopago; }
    public function get_empresa(){ return $this->_empresa; }
    public function get_enviado_a_erp(){ return $this->_enviado_a_erp; }
    public function get_estado(){ return $this->_estado; }
    public function get_fecha_alta(){ return $this->_fecha_alta; }
    public function get_fecha_baja(){ return $this->_fecha_baja; }
    public function get_forma_pago(){ return $this->_forma_pago; }
    public function get_iban(){ return $this->_iban; }
    public function get_id(){ return $this->_id; }
    public function get_modify_date(){ return $this->_modify_date; }
    public function get_modify_user(){ return $this->_modify_user; }
    public function get_num_cuenta(){ return $this->_num_cuenta; }
    public function get_numero_calle(){ return $this->_numero_calle; }
    public function get_observaciones(){ return $this->_observaciones; }
    public function get_poblacion(){ return $this->_poblacion; }
    public function get_sucursal(){ return $this->_sucursal; }
    public function get_tipo_via(){ return $this->_tipo_via; }
    public function get_ultima_operacion(){ return $this->_ultima_operacion; }
    public function get_validado(){ return $this->_validado; }
    
    public function set_baja_logica($value){ $this->_baja_logica = $value; }
    public function set_cod_baja($value){ $this->_cod_baja = $value; }
    public function set_cod_banco($value){ $this->_cod_banco = $value; }
    public function set_code($value){ $this->_code = $value; }
    public function set_create_date($value){ $this->_create_date = $value; }
    public function set_create_user($value){ $this->_create_user = $value; }
    public function set_credito_caution($value){ $this->_credito_caution = $value; }
    public function set_dc($value){ $this->_dc = $value; }
    public function set_delete_date($value){ $this->_delete_date = $value; }
    public function set_delete_user($value){ $this->_delete_user = $value; }
    public function set_descuento($value){ $this->_descuento = $value; }
    public function set_descuento2($value){ $this->_descuento2 = $value; }
    public function set_desea_informacion($value){ $this->_desea_informacion = $value; }
    public function set_dto_prontopago($value){ $this->_dto_prontopago = $value; }
    public function set_empresa($value){ $this->_empresa = $value; }
    public function set_enviado_a_erp($value){ $this->_enviado_a_erp = $value; }
    public function set_estado($value){ $this->_estado = $value; }
    public function set_fecha_alta($value){ $this->_fecha_alta = $value; }
    public function set_fecha_baja($value){ $this->_fecha_baja = $value; }
    public function set_forma_pago($value){ $this->_forma_pago = $value; }
    public function set_iban($value){ $this->_iban = $value; }
    public function set_id($value){ $this->_id = $value; }
    public function set_modify_date($value){ $this->_modify_date = $value; }
    public function set_modify_user($value){ $this->_modify_user = $value; }
    public function set_num_cuenta($value){ $this->_num_cuenta = $value; }
    public function set_numero_calle($value){ $this->_numero_calle = $value; }
    public function set_observaciones($value){ $this->_observaciones = $value; }
    public function set_poblacion($value){ $this->_poblacion = $value; }
    public function set_sucursal($value){ $this->_sucursal = $value; }
    public function set_tipo_via($value){ $this->_tipo_via = $value; }
    public function set_ultima_operacion($value){ $this->_ultima_operacion = $value; }
    public function set_validado($value){ $this->_validado = $value; }
}
