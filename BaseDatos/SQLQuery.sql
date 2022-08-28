use sag;

create table users(
	username		varchar(20) NOT NULL,
    pwd				varchar(20) NOT NULL,
    administrador	bit		NOT NULL
);

create table producto (
	id			char(2) not null,
    nombre		varchar(50) not null,
    costo		decimal(15, 3) not null,
	impuesto	decimal(15,3) not null,
    
    primary key (id)
);

create table area (
	id						int not null auto_increment, -- identity
    nombre					varchar(50) not null,
    dimension				decimal(15, 3) not null,
	productoPrincipal		varchar(50) not null,
    
    primary key (id)
);

create table infoLocal ( -- Valores iniciales
	cedulaJuridica		char(10) not null,
    nombreLocal			varchar(50) not null,
    telefono			char(8) not null,
    consecutivoFactura	bigint not null,
    
    primary key (cedulaJuridica)
);

create table empleado (
	cedula			varchar(9) not null,
    nombre			varchar(50) not null,
    apellido1		varchar(50) not null,
    apellido2		varchar(50) not null,
    labor			varchar(100) not null,
    salario			decimal(15,3) not null,
    salarioCargas	decimal(15,3) not null,
    
    primary key (cedula)
);

create table planilla (
	id			int not null auto_increment, -- identity
	mes			tinyInt not null,
    ano			int not null,
    subtotal	decimal(15,3) not null,
    total		decimal(15,3) not null,
    detalles	varchar(200) not null,
    
    primary key (id)
);

create table planillaEmpleado (
	cedulaEmpleado	char(9) not null,
    idPlanilla		int,
    
    primary key (cedulaEmpleado, idPlanilla),
    
    foreign key (cedulaEmpleado)
		references empleado (cedula),
	foreign key (idPlanilla)
		references planilla (id)
);

create table factura (
	id				int not null auto_increment, -- Consecutivo
    nombreCliente	varchar(50) not null,
    costoTotal		decimal(15, 3) not null,
	impuesto		decimal(15,3) not null,
    fecha			date not null, -- YYYY-MM-DD
    
    primary key (id)
);

create table facturaProductos (
	idFactura		int not null auto_increment,
    idProducto		char(2),
    
    primary key (idFactura, idProducto),
    
    foreign key (idFactura)
		references factura (id),
	foreign key (idProducto)
		references productos (id)
);


-- ************************* Inserts *************************
-- Insertar empleados

insert into empleado(cedula, nombre, apellido1, apellido2, labor, salario) values 
    ('116349476', 'Juan', 'Perez', 'Sanchez', 'Agricultor', 400000, (400000*1.5)),
    ('116328193', 'Pedro', 'Lopez', 'Sanchez', 'Agricultor', 350000, (350000*1.5)),
    ('290834090', 'Luis', 'Sanchez', 'Lopez', 'Administrador', 1000000, (1000000*1.5)),
    ('348568249', 'Lucia', 'Acosta', 'Fernandez', 'Agricultor', 400000, (400000*1.5)),
    ('455464585', 'Enrique', 'Ruiz', 'Lopez', 'Asistente', 250000, (250000*1.5)),
    ('541682035', 'Marta', 'Vidal', 'Gomez', 'Gerente', 700000, (700000*1.5)),
    ('548731215', 'Martin', 'Martinez', 'Garcia', 'Agricultor', 400000, (400000*1.5)),
    ('751512023', 'Maria', 'Cruz', 'Marin', 'Ingeniero', 900000, (900000*1.5)),
    ('105465122', 'Fernando', 'Leon', 'Soto', 'Propietario', 2000000, (2000000*1.5)),
    ('232405466', 'Mario', 'Molina', 'Rodriguez', 'Agricultor', 400000, (400000*1.5));

-- Insertar areas

insert into area (nombre, dimension, productoPrincipal) values
	('El Guarco', 500, 'Papa'),
    ('Limon', 300, 'Banano'),
    ('Heredia Centro', 200, 'Cafe'),
    ('San Jose Centro', 250, 'Cafe'),
    ('Ochomogo', 700, 'Papa'),
    ('Cartago Centro', 500, 'Cebolla'),
    ('Poas', 200, 'Fresa'),
    ('Puerto Viejo', 400, 'Cacao'),
    ('Sarapiqui', 800, 'Pina'),
    ('Liberia', 400, 'Arroz');

-- ************************* Procedimientos y Funciones *************************
-- Funcion que ingresa datos de planilla incluyendo idEmpleado para tabla planillaEmpleado
-- retorna 0 si hubo fallo, 1 si se insertó correctamente
DELIMITER $$
create function ingresaPlanillaEmpleado(tempEmpleadoID char(9), tempPlanillaID int) RETURNS bit
	begin
		declare tempPlanillaID int;
		if not exists(select * from empleado where empleado.cedula = tempEmpleadoID) then
			return 0;
        else
            insert into planillaEmpleado(cedulaEmpleado, idPlanilla)  
				values (tempEmpleadoID, tempPlanillaID);
            return 1;
	end if;
    end$$;
DELIMITER ;

-- Funcion que recibe un user y password y verifica si está en la tabla y de que tipo es (Admin u Operador)
DELIMITER $$
create function checkLogin(tempUsername varchar(10), tempPwd varchar(10), isAdmin bit) -- 1 = Admin / 0 = Operador
RETURNS bit
BEGIN
   RETURN EXISTS(select * from users where username = tempUsername and pwd = tempPwd and administrador = isAdmin);
end$$;
DELIMITER ;


insert into users(username, pwd, administrador) values ('jm','1234', 1);
select * from users;
select checkLogin('jm', '1234', 1);

select ingresaPlanillaEmpleado('116349476', 1);


select * from empleado;
select * from planilla;
select * from planillaEmpleado;

insert into planilla(mes, ano, subtotal, total, detalles)
 values	(1, 2022, 800000, 1200000, 'Mes Enero');

-- ************************* Vistas *************************
-- Vista que reune nominas y la información de los empleados
create view verPlanillaElegida as
select e.cedula, e.nombre, e.apellido1, e.apellido2, e.labor, e.salario, 
p.mes, p.ano, p.subtotal, p.total, p.detalles from planilla p 
	inner join planillaEmpleado pe on p.id = pe.idPlanilla 
    inner join empleado e on pe.cedulaEmpleado = e.cedula;

select * from verPlanillaElegida;