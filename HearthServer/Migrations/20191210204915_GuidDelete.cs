using System;
using Microsoft.EntityFrameworkCore.Migrations;

namespace HearthServer.Migrations
{
    public partial class GuidDelete : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AlterColumn<string>(
                name: "Serial",
                table: "RegisteredDevices",
                nullable: true,
                oldClrType: typeof(Guid),
                oldType: "TEXT");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AlterColumn<Guid>(
                name: "Serial",
                table: "RegisteredDevices",
                type: "TEXT",
                nullable: false,
                oldClrType: typeof(string),
                oldNullable: true);
        }
    }
}
