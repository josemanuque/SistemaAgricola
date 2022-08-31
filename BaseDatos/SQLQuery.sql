create schema sag;

use sag;


SET SQL_SAFE_UPDATES = 0;

create table users(
	username		varchar(20) NOT NULL,
    pwd				varchar(20) NOT NULL,
    tipo			int not null  -- 0 operativo, 1 admin 2 todos
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
    consecutivoFactura	int not null,
    consecutivoPlanilla	int not null,
    
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
	id			int not null, -- identity
	mes			int not null,
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
	id				int not null, -- Consecutivo
    nombreCliente	varchar(50) not null,
    subtotal		decimal(15, 3) not null,
    total			decimal(15, 3) not null,
	impuesto		decimal(15,3) not null,
    dia				int not null,
    mes				int not null,
    ano				int not null,
    
    primary key (id)
);

create table facturaArea (
	idArea 		int not null,
    idFactura	int not null,
    
    primary key (idArea, idFactura),
    
    foreign key  (idArea)
		references area (id),
	foreign key (idFactura)
		references factura (id)
);

create table facturaProductos (
	idFactura		int not null,
    idProducto		char(2) not null,
    cantidad		int not null,
    
    primary key (idFactura, idProducto),
    
    foreign key (idFactura)
		references factura (id),
	foreign key (idProducto)
		references producto (id)
);


-- ************************* Inserts *************************
-- Inserta usuarios
insert into users(username, pwd, tipo) values ('jm','1234', 1) , ('mq', '1234', 0);

-- Inserta valores del local
insert into infolocal(cedulaJuridica, nombreLocal, telefono, consecutivoFactura, consecutivoPlanilla)
	values ('1329853484', 'Local Central', '23435322', 0, 0);

-- Insertar empleados
insert into empleado(cedula, nombre, apellido1, apellido2, labor, salario, salarioCargas) values 
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

-- Procedimiento que aumenta el consecutivoPlanilla en infoLocal
DELIMITER $$
create procedure aumentaConsecutivoPlanilla()
	begin
		update infoLocal set infolocal.consecutivoPlanilla = (infolocal.consecutivoPlanilla + 1);
    end$$;
DELIMITER ;

-- Procedimiento que guarda los datos iniciales de planilla nueva. 
-- Aumenta el consecutivo al inicio.
DELIMITER $$
create procedure ingresaPlanilla(mes int, ano int, detalles varchar(200))
	begin
		declare consecutivo int;
        call aumentaConsecutivoPlanilla;
        set consecutivo = (select consecutivoPlanilla from infoLocal);
        insert into planilla(id, mes, ano, subtotal, total, detalles) values (consecutivo, mes, ano, 0, 0, detalles);
    end$$;
DELIMITER ;

-- Procedimiento que ingresa datos de planillaEmpleado, actualiza planilla con totales y subtotales
DELIMITER $$
create procedure ingresaPlanillaEmpleado(tempEmpleadoID char(9))
	begin
		declare consecutivo int;
        declare tempSubtotal decimal(15,3);
        declare tempTotal decimal (15,3);
        
        set consecutivo = (select consecutivoPlanilla from infoLocal);
		insert into planillaEmpleado(cedulaEmpleado, idPlanilla)  
			values (tempEmpleadoID, consecutivo);
	
        set tempSubtotal = (select sum(salario) from empleado e inner join planillaEmpleado pe 
			on  pe.cedulaEmpleado = e.cedula 
            where pe.idPlanilla = consecutivo and pe.cedulaEmpleado = e.cedula);
		set tempTotal = (select sum(salarioCargas) from empleado e inner join planillaEmpleado pe 
			on  pe.cedulaEmpleado = e.cedula 
            where pe.idPlanilla = consecutivo and pe.cedulaEmpleado = e.cedula);
        
        update planilla set planilla.subtotal = tempSubtotal where planilla.id = consecutivo;
        update planilla set planilla.total = tempTotal where planilla.id = consecutivo;
    end$$;
DELIMITER ;

-- Procedimiento que recibe una planilla y retorna los detalles de los empleados en esta
DELIMITER $$
create procedure verPlanillaElegida(idPlanilla int)
	begin
		select e.cedula, e.nombre, e.apellido1, e.apellido2, e.labor, e.salario, 
		p.mes, p.ano, p.subtotal, p.total, p.detalles from planilla p 
			inner join planillaEmpleado pe on p.id = pe.idPlanilla 
			inner join empleado e on pe.cedulaEmpleado = e.cedula where p.id = idPlanilla;
    end $$;
DELIMITER ;
call verPlanillaElegida(1);
-- ---------- Procedimientos Facturacion ---------------------

-- Procedimiento que aumenta el consecutivoPlanilla en infoLocal
DELIMITER $$
create procedure aumentaConsecutivoFactura()
	begin
		update infoLocal set infolocal.consecutivoFactura = (infolocal.consecutivoFactura + 1);
    end$$;
DELIMITER ;

-- Procedimiento que guarda los datos iniciales de planilla nueva. 
-- Aumenta el consecutivo al inicio.
DELIMITER $$
create procedure ingresaFactura(nombreCliente varchar(200), dia int, mes int, ano int, idArea int)
	begin
		declare consecutivo int;
        call aumentaConsecutivoFactura;
        set consecutivo = (select consecutivoFactura from infoLocal);
        insert into factura(id, dia, mes, ano, nombreCliente, subtotal, total, impuesto) values (consecutivo, dia, mes, ano, nombreCliente, 0, 0, 0);
        insert into facturaArea (idArea, idFactura) values (idArea, consecutivo);
    end$$;
DELIMITER ;


call ingresaFactura('Manuel', 1, 2, 2023, 1);

-- Procedimiento que ingresa datos de planillaEmpleado, actualiza planilla con totales y subtotales
DELIMITER $$
create procedure ingresaProductoFactura(tempProductoID char(2), cantidad int)
    begin
        declare consecutivo int;
        declare tempSubtotal decimal(15,3);
        declare tempTotal decimal (15,3);
        declare tempImpuesto decimal (15,3);

        set consecutivo = (select consecutivoFactura from infoLocal);
        insert into facturaProductos(idFactura, idProducto, cantidad)
                values (consecutivo, tempProductoID, cantidad);


        set tempSubtotal = (select sum((p.costo * fp.cantidad)) from producto p inner join facturaProductos fp 
            on  p.id = fp.idProducto
            where fp.idFactura = consecutivo);
        set tempImpuesto = (select sum((p.impuesto * fp.cantidad)) from producto p inner join facturaProductos fp 
            on  p.id = fp.idProducto
            where fp.idFactura = consecutivo);
        set tempTotal = (tempSubtotal + tempImpuesto);

        update factura set factura.subtotal = tempSubtotal where factura.id = consecutivo;
        update factura set factura.impuesto = tempImpuesto where factura.id = consecutivo;
        update factura set factura.total = tempTotal where factura.id = consecutivo;
    end$$;
DELIMITER ;


-- Procedimiento que muestra el balance mensual de un año

DELIMITER $$
create procedure balanceMensual(inputYear int)
	begin
		select coalesce(p.mes, f.mes) as month, ifnull(p.total, 0) as totalNominaCargas, ifnull(f.subtotal, 0) 
			as subtotalVentas, ifnull(f.impuesto, 0) as totalImpuesto, (ifnull(f.subtotal, 0) - ifnull(p.total, 0)) as Balance from
			(select mes, sum(total) as total from planilla 
				where ano = inputYear
				group by mes) as p
			left join
			(select mes, sum(subtotal) as subtotal, sum(impuesto) as impuesto from factura 
				where ano = inputYear
				group by mes) as f on p.mes = f.mes where p.total != 0 or f.subtotal != 0
		union
		select coalesce(p.mes, f.mes) as month, ifnull(p.total, 0) as totalNominaCargas, ifnull(f.subtotal, 0) 
			as subtotalVentas, ifnull(f.impuesto, 0) as totalImpuesto, (ifnull(f.subtotal, 0) - ifnull(p.total, 0)) as Balance from
			(select mes, sum(total) as total from planilla 
				where ano = inputYear
				group by mes) as p
			right join
			(select mes, sum(subtotal) as subtotal, sum(impuesto) as impuesto from factura 
				where ano = inputYear
				group by mes) as f on p.mes = f.mes where p.total != 0 or f.subtotal != 0 order by month;
    end$$;
DELIMITER ;



DELIMITER $$
create procedure generarFactura(idFacturaTemp int)
	select * from factura f inner join infoLocal

    end$$;
DELIMITER ;
-- ************************* Llamadas a Procedimientos *************************
call ingresaPlanilla(2, 2021, 'Pago Febrero');
call ingresaPlanillaEmpleado('116349476');

call ingresaProductoFactura('3B');

select * from facturaProductos;
select * from producto;
select * from factura;
select (sum(impuesto)  / count(impuesto)) from producto;
select * from empleado;
select * from planilla;
select * from planillaEmpleado;
select * from infoLocal;

select * from balanceAnual;
call balanceMensual(2022);


-- ************************* Vistas *************************

create view balanceAnual as
	select coalesce(p.ano, f.ano) as year, ifnull(p.total, 0) as totalNominaCargas, ifnull(f.subtotal, 0) 
		as subtotalVentas, ifnull(f.impuesto, 0) as totalImpuesto, (ifnull(f.subtotal, 0) - ifnull(p.total, 0)) as Balance from
		(select ano, sum(total) as total from planilla 
			group by ano) as p
		left join
		(select ano, sum(subtotal) as subtotal, sum(impuesto) as impuesto 
			from factura 
			group by ano) as f on p.ano = f.ano where p.total != 0 or f.subtotal != 0
	union
    select coalesce(p.ano, f.ano) as month, ifnull(p.total, 0) as totalNominaCargas, ifnull(f.subtotal, 0) 
		as subtotalVentas, ifnull(f.impuesto, 0) as totalImpuesto, (ifnull(f.subtotal, 0) - ifnull(p.total, 0)) as Balance from
		(select ano, sum(total) as total from planilla 
			group by ano) as p
		right join
		(select ano, sum(subtotal) as subtotal, sum(impuesto) as impuesto 
			from factura 
			group by ano) as f on p.ano = f.ano where p.total != 0 or f.subtotal != 0 order by year;




    