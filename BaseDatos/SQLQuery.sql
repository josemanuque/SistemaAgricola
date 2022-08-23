use test;

create table productos (
	id			char(2) not null,
    nombre		varchar(50) not null,
    costo		decimal(15, 3) not null,
	impuesto	decimal(15,3) not null,
    
    primary key (id)
);

create table areas (
	id						char(2) not null,
    nombre					varchar(50) not null,
    dimension				decimal(15, 3) not null,
	idProductoPrincipal		char(2) not null, -- id principal prod producido
    
    primary key (id),
    
    foreign key (idProductoPrincipal)
		references productos (id)
);

create table infoLocal ( -- Valores iniciales
	cedulaJuridica		char(10) not null,
    nombreLocal			varchar(50) not null,
    telefono			char(8) not null,
    consecutivoFactura	bigint not null,
    
    primary key (cedulaJuridica)
);

create table empleados (
	cedula		varchar(9) not null,
    nombre		varchar(50) not null,
    apellido1	varchar(50) not null,
    apellido2	varchar(50) not null,
    labor		varchar(100) not null,
    salario		decimal(15,3) not null,
    
    primary key (cedula)
);

create table planilla (
	id			int not null auto_increment, -- identity
	mes			tinyInt not null,
    año			int not null,
    subtotal	decimal(15,3) not null,
    total		decimal(15,3) not null,
    detalles	varchar(200) not null,
    
    primary key (id)
);

create table planillaEmpleado (
	cedulaEmpleado	char(9) not null,
    idPlanilla		int not null auto_increment,
    
    primary key (cedulaEmpleado, idPlanilla),
    
    foreign key (cedulaEmpleado)
		references empleados (cedula),
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

